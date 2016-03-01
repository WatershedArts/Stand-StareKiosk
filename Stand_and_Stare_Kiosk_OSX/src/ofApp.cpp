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
    ofClear(0, 0, 0);
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
    
    debug.load("font-verdana.ttf", 8);
    canPlay = false;
    flipIdleTimerLatch = false;
    canDrawData = false;
    calibrateScreen = false;
    
    // Go straight into the Operation Mode
    applicationMode = 2;
    videoPlayback = 1;
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
    // Load the Configuration
    appConfiguration.load("config.json");
    
    // Debug Warper
    useWarper = true;
    
    // Make the GUI
    setupGUI();
    
    // Setup the Apps Variables
    appSetup();
    
    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
    
    // Projector Controller
    projectorController.setupProjector("/dev/tty.usbserial"); // Make this a config option
    projectorController.turnOn();
    
    videoData = appConfiguration.getConfig().videos;
    
    // Video System
    videoHandler.setupVideoPlayer(appConfiguration.getConfig().fadeInTime, appConfiguration.getConfig().fadeOutTime,appConfiguration.getConfig().enticerFadeIn);
    
    // Assign Screen
    videoPreviewer.setup(videoData);
    
    // RFID
    rfidReader.setup(appConfiguration.getConfig().RFIDSerialName,2000);
    rfidReader.start();
    
    // Calibration Setup
    calibrationScreen.Setup(appConfiguration.getConfig().maskPoints);
    
    // This is for Posting Data Back to the server
    postData.setup(appConfiguration.getConfig().postHostURL, appConfiguration.getConfig().postExtURL, appConfiguration.getConfig().secretKey);
    
    // For the Enticing Display
    enticer.setupVideoPlayer(appConfiguration.getConfig().enticerFadeIn, appConfiguration.getConfig().enticerFadeOut,appConfiguration.getConfig().fadeOutTime);
    enticer.loadVideo(appConfiguration.getConfig().enticerVideoUrl);
    enticer.playVideo();
    
    // Setup Screen Warper
    if (useWarper) {
        setupWarper();
    }
    
    setupListeners();
}
//--------------------------------------------------------------
void ofApp::update()
{
    // Update the classes
    idleTimer.Update();
    videoHandler.updateVideo();
    enticer.updateVideo();
    donationReader.update();

    if(applicationMode == 1) {
        videoPreviewer.update();
    }
    
    // Starts the Idle Timer which fires the enticer visuals
    if (!flipIdleTimerLatch && videoHandler.hasVideoFinished()) {
        idleTimer.Start();
        flipIdleTimerLatch = true;
    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    // Set the background color
    ofBackground(ofColor::black);
    
    // Calibration Mode
    if (applicationMode == 0) {
        calibrationScreen.Draw();
    }
    else if(applicationMode == 1) {
        DrawAssigningScreen();
        debug.drawString(videoDetails, 10, 10);
    }
    else if(applicationMode == 2) {
        // Draw the Videos and other obeject to the Screen Warper
        if (useWarper) {
            screenFbo.begin();
            ofClear(0);
        }
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        enticer.drawVideo();
        videoHandler.drawVideo();
        ofDisableBlendMode();
        ofPopStyle();
//        objects.drawObjects();
        
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
        }
        
////        videoHandler.drawVideo();
//        timerVisualisation.setCurrentDuration(200-ofMap(videoHandler.getTimeLeft(),0.00,1.00,200,0));
//        timerVisualisation.draw(0, 0);
    }
    // Draw the debug data from the video Files
    if (canDrawData) {
        ofPushStyle();
        gui->draw();
        DrawDebugData();
        enticer.drawTimeline(ofGetHeight()*0.9);
        ofPopStyle();
    }
}
//--------------------------------------------------------------
void ofApp::DrawDebugData()
{
    stringstream debugData;
    debugData << "|------------------------------" << endl;
    debugData << "| Stand and Stare Jukebox" << endl;
    debugData << "|------------------------------" << endl;
    debugData << "| Video Player" << endl;
    debugData << "|------------------------------" << endl;
    debugData << videoHandler.getStringStream() << endl;
    debugData << rfidReader.getDebugString() << endl;
    debugData << enticer.getStringStream() << endl;
//    debugData << "|------------------------------" << endl;
//    debugData << "| Donation Reader" << endl;
//    debugData << "|------------------------------" << endl;
//    debugData << donationReader.getStringStream() << endl;
    debugData << "|------------------------------" << endl;
    debugData << "| Post" << endl;
    debugData << "|------------------------------" << endl;
    debugData << postData.getDebug() << endl;
    
    ofSetColor(ofColor::white);
    debug.drawString(debugData.str(), 10, 10);
}
//--------------------------------------------------------------
void ofApp::DrawAssigningScreen()
{
    videoPreviewer.draw();
}
//--------------------------------------------------------------
void ofApp::exit()
{
    postData.close();
    donationReader.close();
    videoPreviewer.close();
    rfidReader.stop();
    
    bool debugProjector = false;
    if (debugProjector) {
        projectorController.turnOff();
    }
    
    projectorController.close();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {;
        case ' ':
            canDrawData = !canDrawData;
            gui->setVisible(canDrawData);
            break;
        case 's':
            videoHandler.stopVideo();
            enticer.playVideo();
            break;
        case 'd':
            postData.postDonation();
            break;
        case 'g':
            cout << projectorController.getBulbTime() << endl;
            break;
        case 'r':
            rfidReader.simulateTagRemoval();
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
        videoPreviewer.mouseOver(x, y);
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
                    calibrationScreen.MousePressed(x, y, button);
                }
                else {
                }
            }
            else {
                calibrationScreen.MousePressed(x, y, button);
            }
        }
    }
    else if(applicationMode == 1) {
        videoPreviewer.mousePressed(x, y);
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
//    if(msg.message == "Idle Timer Finished") {
//        cout << "Timer Complete" << endl;
//    }
//    else if(msg.message == "Donation") {
//        // If we get a donation tell the server
//        postData.postDonation();
//    }
//    else if(msg.message == "Forced Stop") {
//        // If the video is stopped before the end it will fire a false code to the server
//        cout << "A Card has been removed whilst video is playing!!!" << endl;
//        postData.postVideo("1",videoCode,videoHandler.getPlayPercentage(),false);
//    }
//    else if(msg.message == "Natural Stop") {
//        // Like wise if the video is finsihes normally it will fire a true code to the server
//        postData.postVideo("1",videoCode,100,true);
//        enticer.playVideo();
//    }
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
    ofAddListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
    
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
    ofRemoveListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
    
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
        for (int i = 0; i < videoData.size(); i++) {
            if (tag == videoData[i].RFIDkey) {
                videoDetails = videoData[i].getData();
            }
        }
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
    }
}
//--------------------------------------------------------------
void ofApp::tagRemoved(string &tag)
{
    videoPlayback = 1;
    ofLogWarning() << tag;
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
{
    
}
//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target->is("Clear Mask")) {
        calibrationScreen.ClearPoints();
    }
    else if(e.target->is("Save Mask")) {
        calibrationScreen.Save();
    }
    else if(e.target->is("Show Coordinates")) {
        calibrationScreen.ShowCoordinates(e.target->getEnabled());
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
{
    
}
//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    
}



