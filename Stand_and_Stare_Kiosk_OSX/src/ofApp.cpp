#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setupWarper()
{
    // Set the Screen Sizes
    int screenWidth = ofGetScreenWidth();
    int screenHeight = ofGetScreenHeight();
    
    showWarper = false;
    
    // Allocate Warper FBO
    screenFbo.allocate(screenWidth, screenHeight);
    screenFbo.begin();
        ofClear(0);
    screenFbo.end();
    
    vector <ofVec3f> screenCoords = calibrationScreen.getCoordinates();
    
    // Set Screen Warper Positions
    screenWarper.setSourceRect(ofRectangle(0,0,screenWidth,screenHeight));
    screenWarper.setTopLeftCornerPosition(screenCoords[0]);
    screenWarper.setTopRightCornerPosition(screenCoords[3]);
    screenWarper.setBottomRightCornerPosition(screenCoords[2]);
    screenWarper.setBottomLeftCornerPosition(screenCoords[1]);
    screenWarper.setup();
}
//--------------------------------------------------------------
void ofApp::appSetup()
{
    // Make the Window the Screen Size
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    
    // Set Fullscreen
    ofSetFullscreen(appConfiguration.getConfig().setFullscreen);

    // Set the Logging Level
    ofSetLogLevel(OF_LOG_WARNING);
    
    logo.load("photos/logo.png");
    titleFont.load("MuseoSans_500.otf", 30);
    debug.load("font-verdana.ttf", 8);
    canPlay = false;
    flipIdleTimerLatch = false;
    canDrawData = false;
    calibrateScreen = false;
    
    // Go straight into the Operation Mode
    applicationMode = 2;
    videoPlayback = 1;
    timesUsedToday = 0;

}
//--------------------------------------------------------------
// *
// *    This allows key strokes to be sent over SSH
// *
//--------------------------------------------------------------
void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}
//--------------------------------------------------------------
void ofApp::setup()
{
    // For Bundling App
    ofSetDataPathRoot("../Resources/data/");
    
    // Load the Configuration
    appConfiguration.loadConfig(ofToDataPath("config.json"));
    appConfiguration.printConfiguration();
    appConfiguration.loadVideoConfig(ofToDataPath("videoConfig.json"));
    appConfiguration.printConfiguration();
    
    // Debug Warper
    useWarper = appConfiguration.getConfig().useWarper;
    
    // Make the GUI
    setupGUI();
    
    // Setup the Apps Variables
    appSetup();
    
    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
    
    arduino.setup("/dev/tty.usbmodem1421",appConfiguration.getConfig().rfidDelay);
    
    // Projector Controller
    projectorController.setupProjector(appConfiguration.getConfig().projectorSerialName);
    projectorController.turnOn();
    
    videoData = appConfiguration.getVideoConfig();
    
    // Video System
    videoHandler.setupVideoPlayer(appConfiguration.getConfig().fadeInTime,
                                  appConfiguration.getConfig().fadeOutTime,
                                  appConfiguration.getConfig().enticerFadeIn
                                  );
    
    tagAssigner.setup(videoData);
    
    // RFID
    rfidReader.setup(appConfiguration.getConfig().RFIDSerialName,
                     appConfiguration.getConfig().rfidDelay
                     );
    
    // Calibration Setup
    calibrationScreen.setup(appConfiguration.getConfig().maskPoints);
    
    // This is for Posting Data Back to the server
    postData.setup(appConfiguration.getConfig().postHostURL,
                   appConfiguration.getConfig().postExtURL,
                   appConfiguration.getConfig().secretKey
                   );
    
    // For the Enticing Display
    enticer.setupVideoPlayer(appConfiguration.getConfig().enticerFadeIn,
                             appConfiguration.getConfig().enticerFadeOut,
                             appConfiguration.getConfig().fadeOutTime
                             );
    
    enticer.loadVideo(appConfiguration.getConfig().enticerVideoUrl);
    
    donationReader.setup("This");
    
    // Setup Screen Warper
    if (useWarper) {
        setupWarper();
    }
    
    // Variables for the Units Id
    unitName = appConfiguration.getConfig().unitName;
    unitId = appConfiguration.getConfig().unitId;
    
    splashScreen.load("photos/logo.png",unitName,2500);
    initLoad = false;
    
    setupListeners();
}
//--------------------------------------------------------------
void ofApp::update()
{
    // Update the classes
    idleTimer.update();
    videoHandler.updateVideo();
    enticer.updateVideo();
    donationReader.update();
    splashScreen.update();
    arduino.update();
    rfidReader.update();
    

    if (splashScreen.isDone() && !initLoad) {
        enticer.playVideo();
        initLoad = true;
    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    // Set the background color
    ofBackground(ofColor::black);
    
    if (!splashScreen.isDone()) {
        splashScreen.draw();
    }
    else {
        // Calibration Mode
        if (applicationMode == 0) {
            calibrationScreen.draw();
        }
        else if(applicationMode == 1) {
            drawAssigningScreen();
        }
        else if(applicationMode == 2) {
            donationReader.draw(0, 0);
            // Draw the Videos and other obeject to the Screen Warper
            if (useWarper) {
                screenFbo.begin();
                ofClear(0);
            }

            ofPushStyle();
            enticer.drawVideo();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            videoHandler.drawVideo();
            ofDisableBlendMode();
            ofPopStyle();
            
            if (useWarper) {
                screenFbo.end();
                
                // Do the Screen Warping
                ofMatrix4x4 mat = screenWarper.getMatrix();
                ofPushMatrix();
                ofMultMatrix(mat);
                ofSetColor(ofColor::white);
                screenFbo.draw(0,0);
                ofPopMatrix();
                
                if (showWarper) {
                    ofSetColor(ofColor::ivory);
                    screenWarper.drawQuadOutline();
                    
                    ofSetColor(ofColor::blueSteel);
                    screenWarper.drawCorners();
                    
                    ofSetColor(ofColor::blueViolet);
                    screenWarper.drawHighlightedCorner();
                }
                videoHandler.drawCalibrationQuads();
                enticer.drawCalibrationQuads();
            }
        }
        // Draw the debug data from the video Files
        if (canDrawData) {
            ofPushStyle();
            ofSetColor(ofColor::white);
            logo.draw(5,5,logo.getWidth()*0.5,logo.getHeight()*0.5);
            drawDebugData();
            enticer.drawTimeline(ofGetHeight()*0.8);
            videoHandler.drawTimeline(ofGetHeight()*0.9);
            ofPopStyle();
        }
    }
//    if (!disappear) {
//        if (fade.isRunning()) {
//            ofSetColor(fade.update());
//        }
//        else if (!fade.isRunning() && !splashScreenTimer.hasTimerFinished()) {
//            ofSetColor(ofColor::white);
//        }
//    
//        int screenCenterX = (ofGetWidth()*0.5);
//        int screenCenterY = (ofGetHeight()*0.5);
//        int offsetCenterX = (logo.getWidth()*0.5);
//        int offsetCenterY = (logo.getHeight()*0.5);
//        
//        logo.draw((screenCenterX-offsetCenterX),(screenCenterY-offsetCenterY));
//        ofRectangle r = titleFont.getStringBoundingBox(unitName, 0, 0);
//        titleFont.drawString(unitName, (screenCenterX-(r.getWidth()*0.5)), 5+(offsetCenterY+screenCenterY+(r.getHeight())));
//    }
}
//--------------------------------------------------------------
void ofApp::drawDebugData()
{
    stringstream debugData;
    debugData << "|------------------------------" << endl;
    debugData << "| Stand and Stare Jukebox" << endl;
    debugData << "| Times Used Today: " << timesUsedToday << endl;
    debugData << videoHandler.getStringStream() << endl;
    debugData << enticer.getStringStream() << endl;
    debugData << rfidReader.getDebugString() << endl;
    debugData << postData.getDebug() << endl;
    
    ofSetColor(ofColor::white);
    debug.drawString(debugData.str(), 10, logo.getHeight()*0.5+15);
}
//--------------------------------------------------------------
void ofApp::drawAssigningScreen()
{
    tagAssigner.draw();
}
//--------------------------------------------------------------
void ofApp::exit()
{
    postData.close();
    donationReader.close();
    enticer.stop();
    
    bool debugProjector = false;
    if (debugProjector) {
        projectorController.turnOff();
    }
    
    projectorController.close();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case ' ':
            canDrawData = !canDrawData;
            gui->setVisible(canDrawData);
            break;
        case 'd':
            donationReader.simulateDonation();
            postData.postDonation();
            break;
        case 'r':
            rfidReader.simulateTagRemoval();
            arduino.simulateRemoval();
            break;
        default:
            break;
    }
    
    for (int i = 0; i < videoData.size(); i++) {
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
void ofApp::mouseMoved(int x, int y)
{
    if(applicationMode == 1) {
        tagAssigner.mouseOver(x, y);
    }
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (applicationMode == 0) {
        if (calibrateScreen) {
            if (gui->getVisible()) {
                guiWindow = ofRectangle(gui->getPosition().x,gui->getPosition().y,gui->getWidth(),gui->getHeight());
                
                // This function is to stop the mouse Click through the GUI Window
                if (!guiWindow.inside(x, y)) {
                    calibrationScreen.mousePressed(x, y, button);
                }
                else {
                }
            }
            else {
                calibrationScreen.mousePressed(x, y, button);
            }
        }
    }
    else if(applicationMode == 1) {
        tagAssigner.mousePressed(x, y, button);
    }
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
    
}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
    
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
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
    
    ofAddListener(rfidReader.newTag, this, &ofApp::newTagAdded);
    ofAddListener(arduino.rfidTagRemoved, this, &ofApp::tagRemoved);
    
    ofAddListener(arduino.donationSlot1Event,this,&ofApp::gotDonation);
    ofAddListener(arduino.donationSlot2Event,this,&ofApp::gotDonation);
    
    ofAddListener(enticer.enticerStarted, this, &ofApp::enticerVideoStarted);
    ofAddListener(enticer.enticerInterrupted, this, &ofApp::enticerVideoFinished);
}
//--------------------------------------------------------------
void ofApp::removeListeners()
{
    ofRemoveListener(videoHandler.videoStarted, this, &ofApp::videoStarted);
    ofRemoveListener(videoHandler.videoStopped, this, &ofApp::videoFinished);
    ofRemoveListener(videoHandler.videoInterrupted, this, &ofApp::videoInterupted);
    
    ofRemoveListener(rfidReader.newTag, this, &ofApp::newTagAdded);
    ofRemoveListener(arduino.rfidTagRemoved, this, &ofApp::tagRemoved);
    
    ofRemoveListener(arduino.donationSlot1Event,this,&ofApp::gotDonation);
    ofRemoveListener(arduino.donationSlot2Event,this,&ofApp::gotDonation);
    
    ofRemoveListener(enticer.enticerStarted, this, &ofApp::enticerVideoStarted);
    ofRemoveListener(enticer.enticerInterrupted, this, &ofApp::enticerVideoFinished);
}
//--------------------------------------------------------------
void ofApp::videoStarted(string &args)
{
    ofLogWarning() << args;
}
//--------------------------------------------------------------
void ofApp::videoFinished(string &args)
{
    postData.postVideo("1",videoCode,100,true);
    videoPlayback = 1;
    enticer.playVideo();
    ofLogWarning() << args;
}
//--------------------------------------------------------------
void ofApp::videoInterupted(string &args)
{
    postData.postVideo("1",videoCode,videoHandler.getPlayPercentage(),false);
    ofLogWarning() << args;
}
//--------------------------------------------------------------
void ofApp::enticerVideoStarted(string &args)
{
    ofLogWarning() << args;
    videoPlayback = 1;
}
//--------------------------------------------------------------
void ofApp::enticerVideoFinished(string &args)
{
    ofLogWarning() << args;
    if (videoPlayback == 1) {
        enticer.playVideo();
    }
}
//--------------------------------------------------------------
void ofApp::newTagAdded(string &tag)
{
    ofLogWarning() << tag;
    videoPlayback = 0;
    if (applicationMode == 0) {
        ofLogWarning() << "Not Allowed in this Mode";
    }
    else if (applicationMode == 1) {
        tagAssigner.assignNewTag(tag);
    }
    else if (applicationMode == 2) {
        for (int i = 0; i < videoData.size(); i++) {
            if (tag == videoData[i].RFIDkey) {
                videoCode = ofToString(i);
                videoHandler.loadVideo(videoData[i].videoUrl);
                videoHandler.playVideo();
                enticer.stopVideo();
            }
        }
        timesUsedToday++;
    }
}
//--------------------------------------------------------------
void ofApp::tagRemoved(int &v)
{
    videoPlayback = 1;
    ofLogWarning() << v;
    
    // If a Card is removed then and video is playing stop and fire post event
    if(videoHandler.isVideoPlaying()) {
        videoHandler.interruptVideo();
        enticer.playVideo();
    }
    else {
        cout << "A Card has been removed!" << endl;
        if (!enticer.isVideoPlaying()) {
            enticer.playVideo();
        }
    }
}
//--------------------------------------------------------------
void ofApp::timerStopped(string &timer)
{
    ofLogWarning() << timer;
}
//--------------------------------------------------------------
void ofApp::gotDonation(int &pin)
{
    cout << pin << endl;
}
//--------------------------------------------------------------
// *
// * GUI
// *
//--------------------------------------------------------------
void ofApp::setupGUI()
{
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT);
    gui->addHeader("Stand & Stare");
    gui->addFRM(1.0f);
    gui->addBreak();
    vector<string> AppMode = {"CALIBRATION MODE",
        "ASSIGNING MODE",
        "OPERATION MODE"};
    
    gui->addDropdown("App Mode", AppMode);
    gui->addButton("Reload Video Data");
    
    if (useWarper) {
        gui->addBreak();
        gui->addToggle("Show Warper");
        gui->addBreak();
        ofxDatGuiFolder * CalibrationFolder = gui->addFolder("Calibration",ofColor::blueSteel);
        CalibrationFolder->addToggle("Enable Mask Creation");
        CalibrationFolder->addButton("Clear Mask");
        CalibrationFolder->addButton("Save Mask");
        CalibrationFolder->addToggle("Show Coordinates");
        CalibrationFolder->addBreak();
        gui->addBreak();
        ofxDatGuiFolder * VideoFolder = gui->addFolder("Video",ofColor::maroon);
        VideoFolder->addToggle("Show Primary Warper Quads");
        VideoFolder->addToggle("Show Secondary Warper Quads");
        VideoFolder->addToggle("Show Quads");
        gui->addBreak();
    }
    gui->addFooter();
    
    // Listeners
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    gui->onMatrixEvent(this, &ofApp::onMatrixEvent);
    
    gui->setVisible(false);
    guiWindow = ofRectangle(gui->getPosition().x,gui->getPosition().y,gui->getWidth(),gui->getHeight());
}
//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{   }
//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target->is("Clear Mask")) {
        calibrationScreen.clearPoints();
    }
    else if(e.target->is("Save Mask")) {
        calibrationScreen.save();
    }
    else if(e.target->is("Show Coordinates")) {
        calibrationScreen.showCoordinates(e.target->getEnabled());
    }
    else if(e.target->is("Enable Mask Creation")) {
        calibrateScreen = e.target->getEnabled();
    }
    else if(e.target->is("Show Secondary Warper Quads")){
        videoHandler.showSecondaryQuad(e.target->getEnabled());
        enticer.showSecondaryQuad(e.target->getEnabled());
    }
    else if(e.target->is("Show Primary Warper Quads")) {
        videoHandler.showPrimaryQuad(e.target->getEnabled());
        enticer.showPrimaryQuad(e.target->getEnabled());
    }
    else if(e.target->is("Show Warper")) {
        showWarper = e.target->getEnabled();
    }
    else if(e.target->is("Reload Video Data")) {
        appConfiguration.loadVideoConfig("videoConfig.json");
        videoData.clear();
        videoData = appConfiguration.getVideoConfig();
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
        if (e.target->getLabel() == "CALIBRATION MODE") {
            applicationMode = 0;
            gui->getFolder("Calibration")->expand();
        }
        else if (e.target->getLabel() == "ASSIGNING MODE") {
            applicationMode = 1;
        }
        else if (e.target->getLabel() == "OPERATION MODE") {
            applicationMode = 2;
        }
    }
}
//--------------------------------------------------------------
void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{   }
//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{   }