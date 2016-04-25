#include "ofApp.h"
#pragma mark - OF Methods
//--------------------------------------------------------------
void ofApp::setup()
{
    // Load the Configuration
    appSettings.loadConfig("config.json");
    appSettings.loadVideoConfig("videoConfig.json");

    // Resize the Window
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());

    // Set Fullscreen
    ofSetFullscreen(appSettings.getConfig().setFullscreen);
    
    // Set the Logging Level
    ofSetLogLevel(OF_LOG_WARNING);
    
    delayTimer.setup(2500, "Delay Timer", false);
    
    // Setup RFID
    rfidReader.setup(appSettings.getConfig().RFIDSerialName,
                     appSettings.getConfig().rfidDelay
                     );
    rfidReader.start();
    
    
    // Video System
    videoHandler.setupVideoPlayer(appSettings.getConfig().fadeInTime,
                                  appSettings.getConfig().fadeOutTime,
                                  appSettings.getConfig().enticerFadeIn,
                                  appSettings.getConfig().enticerVideoUrl);
    
    videoHandler.loadVideo(appSettings.getConfig().enticerVideoUrl,true);
//    videoHandler.start();
    // This is for Posting Data Back to the server
    postData.setup(appSettings.getConfig().postHostURL,
                   appSettings.getConfig().postExtURL,
                   appSettings.getConfig().secretKey
                   );
    
    splashScreen.load("photos/logo.png", "this");
    initLoad = false;
    playEnticer = true;
    timesUsedToday = 0;
    
    // What App Mode
    applicationMode = PLAYBACK_MODE;

    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
    
    // Setup the App Objects
    setupGUI();
    
    assigner.setup(appSettings.getVideoConfig());

    setupListeners();
    
    debugFont.load("font-verdana.ttf", 8);
}
//--------------------------------------------------------------
void ofApp::update()
{
    splashScreen.update();
    delayTimer.update();

    if (splashScreen.isDone() && !initLoad) {
        videoHandler.playVideo();
        initLoad = true;
    }
    
    videoHandler.updateVideo();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(ofColor::black);
    if (!splashScreen.isDone()) {
        splashScreen.draw();
    }
    else {
        if (applicationMode == ASSIGNMENT_MODE) {
            assigner.draw();
        }
        else if(applicationMode == PLAYBACK_MODE) {
            videoHandler.drawVideo();
        }

        if (drawGui) {
            stringstream debugData;
            debugData << "|------------------------------" << endl;
            debugData << "| Stand and Stare Jukebox" << endl;
            debugData << "| Times Used Today: " << timesUsedToday << endl;
            debugData << videoHandler.getStringStream() << endl;
            debugData << rfidReader.getDebugString() << endl;
            debugData << postData.getDebug() << endl;
            ofSetColor(ofColor::white);
            debugFont.drawString(debugData.str(), 10, 15);
            videoHandler.drawTimeline(ofGetHeight()*0.8);
        }
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case ' ':
            drawGui = !drawGui;
            gui->setVisible(drawGui);
            break;
        case 'r':
            rfidReader.simulateTagRemoval();
            break;
        default:
            break;
    }

    for (int i = 0; i < appSettings.getVideoConfig().size(); i++) {
        if (key == ofToChar(ofToString(i))) {
            rfidReader.simulateNewTag(i+1);
        }
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    if (applicationMode == ASSIGNMENT_MODE) {
        assigner.mouseOver(x, y);
    }
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (applicationMode == ASSIGNMENT_MODE) {
        assigner.mousePressed(x, y, button);
    }
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}
//--------------------------------------------------------------
void ofApp::exit()
{
    rfidReader.stop();
//    videoHandler.stop();
    removeListeners();
}
#pragma mark - Terminal Listener
//--------------------------------------------------------------
// *
// *    This allows key strokes to be sent over SSH
// *
//--------------------------------------------------------------
void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}
#pragma mark - Class Listeners
//--------------------------------------------------------------
// *
// * Class Listeners
// *
//--------------------------------------------------------------
void ofApp::setupListeners()
{
    ofAddListener(videoHandler.videoStarted, this, &ofApp::videoStarted);
    ofAddListener(videoHandler.videoStopped, this, &ofApp::videoFinished);
    ofAddListener(videoHandler.videoInterrupted, this, &ofApp::videoInterupted);
    
    ofAddListener(delayTimer.timerStarted,this,&ofApp::delayTimerStarted);
    ofAddListener(delayTimer.timerFinished,this,&ofApp::delayTimerFinished);
    
    ofAddListener(rfidReader.newTag, this, &ofApp::tagAdded);
    ofAddListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
}
//--------------------------------------------------------------
void ofApp::removeListeners()
{
    ofRemoveListener(videoHandler.videoStarted, this, &ofApp::videoStarted);
    ofRemoveListener(videoHandler.videoStopped, this, &ofApp::videoFinished);
    ofRemoveListener(videoHandler.videoInterrupted, this, &ofApp::videoInterupted);
    
    ofRemoveListener(delayTimer.timerStarted,this,&ofApp::delayTimerStarted);
    ofRemoveListener(delayTimer.timerFinished,this,&ofApp::delayTimerFinished);
    
    ofRemoveListener(rfidReader.newTag, this, &ofApp::tagAdded);
    ofRemoveListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
}
//--------------------------------------------------------------
void ofApp::tagAdded(string &tag)
{
    if (applicationMode == ASSIGNMENT_MODE) {
        assigner.assignNewTag(tag);
    }
    else if(applicationMode == PLAYBACK_MODE) {
        videoHandler.stopVideo();
        playEnticer = false;
    
        for (int i = 0; i < appSettings.getVideoConfig().size(); i++) {
            if (tag == appSettings.getVideoConfig()[i].RFIDkey) {
                if (videoHandler.isEnticerPlaying()) {
                    cout << "Enticer is Playing" << endl;
                }
                delayTimer.start();
                cuedVideoUrl = appSettings.getVideoConfig()[i].videoUrl;
                videoCode = ofToString(i);
            }
        }
        timesUsedToday++;
    }
}
//--------------------------------------------------------------
void ofApp::tagRemoved(string &tag)
{
    ofLogWarning() << tag;
    delayTimer.start();
    playEnticer = true;
    
    // If a Card is removed then and video is playing stop and fire post event
    if(videoHandler.isVideoPlaying()) {
        videoHandler.interruptVideo();
    }
}
//--------------------------------------------------------------
void ofApp::videoStarted(string &args)
{

}
//--------------------------------------------------------------
void ofApp::videoFinished(string &args)
{
    if (args == "Enticer") {
//        ofLogWarning() << "Enticer Finished";
//        videoHandler.playVideo();
        delayTimer.start();
    }
    else if (args != "Enticer") {
//        ofLogWarning() << "Not Enticer";
        postData.postVideo("1",videoCode,100,true);
        delayTimer.start();
    }

}
//--------------------------------------------------------------
void ofApp::videoInterupted(string &args)
{
    if (args == "Enticer") {
//        ofLogWarning() << "Enticer Interupted";
        playEnticer = false;
    }
    else if (args != "Enticer") {
//        ofLogWarning() << "Enticer Not Interrupted";
        playEnticer = true;
        postData.postVideo("1",videoCode,videoHandler.getPlayPercentage(),false);
        delayTimer.start();
    }
}
//--------------------------------------------------------------
void ofApp::delayTimerStarted(string &args)
{
//    ofLogWarning() << "Delay Timer Started ";
}
//--------------------------------------------------------------
void ofApp::delayTimerFinished(string &args)
{
    if (playEnticer) {
        videoHandler.loadVideo("",true);
        videoHandler.playVideo();
    }
    else {
        videoHandler.loadVideo(cuedVideoUrl,false);
        videoHandler.playVideo();
    }
}
#pragma mark - GUI
//--------------------------------------------------------------
// *
// * GUI
// *
//--------------------------------------------------------------
void ofApp::setupGUI()
{
    drawGui = false;
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT);
    gui->addHeader("Stand & Stare");
    gui->addFRM(1.0f);
    gui->addBreak();
    vector<string> AppMode = {
        "ASSIGNING MODE",
        "OPERATION MODE"};
    
    gui->addDropdown("App Mode", AppMode);
    gui->addToggle("Draw Video Outline");
    gui->addButton("Reload Video Data");
    gui->addFooter();
    
    // Listeners
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    gui->onMatrixEvent(this, &ofApp::onMatrixEvent);
    
    gui->setVisible(drawGui);
    guiWindow = ofRectangle(gui->getPosition().x,gui->getPosition().y,gui->getWidth(),gui->getHeight());
}
//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{   }
//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target->is("Reload Video Data")) {
    //        appConfiguration.loadVideoConfig("videoConfig.json");
    //        videoData.clear();
    //        videoData = appConfiguration.getVideoConfig();
    }
    else if(e.target->is("Draw Video Outline")) {
        videoHandler.setDrawOutline(e.target->getEnabled());
    }
}
//--------------------------------------------------------------
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{   }
//--------------------------------------------------------------
void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{   }
//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    if(e.target->is("App Mode")) {
        if (e.target->getLabel() == "ASSIGNING MODE") {
            applicationMode = 0;
        }
        else if (e.target->getLabel() == "OPERATION MODE") {
            applicationMode = 1;
        }
    }
}
//--------------------------------------------------------------
void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{   }
//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{   }