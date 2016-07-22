#include "ofApp.h"

#pragma mark - Main OF Functions
//--------------------------------------------------------------
void ofApp::setup()
{
    ofHideCursor();
    
    // For Bundling App
    ofSetDataPathRoot("../Resources/data/");
    ofSetLogLevel(OF_LOG_SILENT);
    ofSetWindowShape(1280, 800);
    
    // Load the Configuration
    appConfiguration.loadConfig("configs/config.json");
    appConfiguration.loadVideoConfig("configs/videoConfig.json");
    
    // Debug Warper
    useWarper = appConfiguration.getConfig().useWarper;
    
    // Make the GUI
    setupGUI();
    
    // Setup the Apps Variables
    appSetup();
    
    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
    
    arduino.setup(appConfiguration.getConfig().arduinoName,appConfiguration.getConfig().rfidDelay);
    
    arduino.setupPins(appConfiguration.getConfig().ledPin1, appConfiguration.getConfig().ledPin2, appConfiguration.getConfig().rfidTIRPin, appConfiguration.getConfig().donationPin1, appConfiguration.getConfig().donationPin2);
    
// Projector Controller
//    projectorController.setupProjector(appConfiguration.getConfig().projectorSerialName);
//    ofSleepMillis(1000);
//    projectorController.turnOn();
    
    videoData = appConfiguration.getVideoConfig();
    
    // Video System
    videoHandler.setupVideoPlayer(appConfiguration.getConfig().fadeInTime,
                                  appConfiguration.getConfig().fadeOutTime,
                                  appConfiguration.getConfig().enticerFadeIn
                                  );
    
    tagAssigner.setup(videoData);
    
//    // RFID
//    rfidReader.setup(appConfiguration.getConfig().RFIDSerialName,
//                     appConfiguration.getConfig().rfidDelay
//                     );
    
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
    if (arduino.isConnected()) {
        arduino.update();    
    }
    
    
//    if (rfidReader.isConnected()) {
        rfidReader.update();
//    }
    
    if (canDrawData) {
        appFPS->update();
        appMode->update();
        calibrationFolder->update();
        showTemplateImage->update();
        reloadVideoData->update();
        videoFolder->update();
        addNewFile->update();
    }
    
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
            
            // Draw the Videos and other obeject to the Screen Warper
            if (useWarper) {
                screenFbo.begin();
                ofClear(0);
            }

            ofPushStyle();
            ofSetColor(255, 255, 255);
            
            if (videoHandler.isVideoPlaying()) {
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                donationReader.draw(0,0);
                ofDisableBlendMode();
            }
            
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            enticer.drawVideo();
            videoHandler.drawVideo();
            ofDisableBlendMode();
            
            if (!videoHandler.isVideoPlaying()) {
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                donationReader.draw(0,0);
                ofDisableBlendMode();
            }
            
        
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
                enticer.drawCalibrationQuads();
            }
        }
        if (showTemplate) {
            donationReader.drawMaskOutline();
        }
        // Draw the debug data from the video Files
        if (canDrawData) {
            ofPushStyle();
            ofSetColor(ofColor::white);
            title->draw();
            appFPS->draw();
            appMode->draw();
            calibrationFolder->draw();
            showTemplateImage->draw();
            reloadVideoData->draw();
            videoFolder->draw();
            addNewFile->draw();
            
            drawDebugData();
            enticer.drawTimeline(ofGetHeight()*0.7);
            videoHandler.drawTimeline(ofGetHeight()*0.8);
            ofPopStyle();
//            donationReader.drawScreens();
        }
    }
}
//--------------------------------------------------------------
void ofApp::exit()
{
    postData.close();
    donationReader.close();
    enticer.stop();
    rfidReader.closeConnection();
    arduino.close();
    ofSleepMillis(1000);
//    bool debugProjector = false;
//    if (debugProjector) {
//    projectorController.turnOff();
//    }
//    
//    projectorController.close();
}
#pragma mark - Key Events
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case ' ':
            canDrawData = !canDrawData;
            if (canDrawData) {
                ofShowCursor();
            }
            else {
                ofHideCursor();
            }
            break;
        case 'Q':
            ofExit();
            break;
        case 'd':
            if (donationReader.canDonate()) {
                int a = 1;
                ofNotifyEvent(donate,a, this);
                postData.postDonation(1);
            }
            break;
        case 'r':
            rfidReader.simulateTagRemoval();
            arduino.simulateRemoval();
            break;
        case '1':
            rfidReader.simulateNewTag(videoData[0].RFIDkey);
            break;
        case '2':
            rfidReader.simulateNewTag(videoData[1].RFIDkey);
            break;
        case '3':
            rfidReader.simulateNewTag(videoData[2].RFIDkey);
            break;
        case '4':
            rfidReader.simulateNewTag(videoData[3].RFIDkey);
            break;
        case '5':
            rfidReader.simulateNewTag(videoData[4].RFIDkey);
            break;
        case '6':
            rfidReader.simulateNewTag(videoData[5].RFIDkey);
            break;
        default:
            break;
    }
    
    
    

    
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
#pragma mark - Mouse Events
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
    if (applicationMode == 0) {     }
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
#pragma mark - Window Events
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
#pragma mark - Extra Draw Functions
//--------------------------------------------------------------
void ofApp::drawDebugData()
{
    stringstream debugData;
    
    ofSetColor(ofColor::white);
    debug.drawString(enticer.getStringStream(), 210, ofGetHeight()-100);
    debug.drawString(videoHandler.getStringStream(), 400, ofGetHeight()-100);
    debug.drawString(rfidReader.getDebugString(), 590, ofGetHeight()-100);
    debug.drawString(arduino.getDebugString(),780,ofGetHeight()-100);
    
    debugData << "| Times Used Today: "  << ofToString(timesUsedToday) << endl;
    debugData << "| Donations Today: "  << ofToString(donationsToday) << endl;
    
    debug.drawString(debugData.str(),970,ofGetHeight()-100);
}
//--------------------------------------------------------------
void ofApp::drawAssigningScreen()
{
    tagAssigner.draw();
}
#pragma mark - Setup and Destroy Listeners
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
    ofAddListener(arduino.connectToRFID,this,&ofApp::reconnectRFID);
    
    ofAddListener(enticer.enticerStarted, this, &ofApp::enticerVideoStarted);
    ofAddListener(enticer.enticerInterrupted, this, &ofApp::enticerVideoFinished);
    
    ofAddListener(donate, this, &ofApp::gotDonation);
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
    
    ofRemoveListener(donate, this, &ofApp::gotDonation);
}
#pragma mark - Listeners
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
        if (videoHandler.isVideoPlaying()) {
//            arduino.simulateGracePeriodInterrupt(); //for keyboard interrupt
            for (int i = 0; i < videoData.size(); i++) {
                if (tag == videoData[i].RFIDkey) {
                    videoCode = ofToString(videoData[i].id);
                    if (tag != previousTagId) {
                        videoHandler.loadVideo(videoData[i].videoUrl);
                        videoHandler.playVideo();
                        enticer.stopVideo();
                    }
                    else {
                        cout << "Already Playing This Video" << endl;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < videoData.size(); i++) {
                if (tag == videoData[i].RFIDkey) {
                    videoCode = ofToString(videoData[i].id);
                    videoHandler.loadVideo(videoData[i].videoUrl);
                    videoHandler.playVideo();
                    enticer.stopVideo();
                }
            }
        }
        
        previousTagId = tag;
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
void ofApp::reconnectRFID(int &args)
{
    cout << "Got Reconnection Event" << endl;
    rfidReader.setup(appConfiguration.getConfig().RFIDSerialName,appConfiguration.getConfig().rfidDelay);
}
//--------------------------------------------------------------
void ofApp::timerStopped(string &timer)
{
    ofLogWarning() << timer;
}
//--------------------------------------------------------------
void ofApp::gotDonation(int &pin)
{
    if (donationReader.canDonate()) {
        cout << "Got Donation" << endl;
        postData.postDonation(pin);
        
        if (!videoHandler.isVideoPlaying()) {
            donationReader.donatedNoFilmPlaying();
        }
        else {
            donationReader.donatedFilmPlaying();
        }
        donationsToday++;
    }
}
#pragma mark - Extra Setup Routine
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
    titleFont.load("fonts/MuseoSans_500.otf", 30);
    debug.load("fonts/MuseoSans_500.otf", 8);
    canPlay = false;
    flipIdleTimerLatch = false;
    canDrawData = false;
    calibrateScreen = false;
    showTemplate = false;
    stopLoad = false;
    
    
    // Go straight into the Operation Mode
    applicationMode = 2;
    videoPlayback = 1;
    timesUsedToday = 0;
    donationsToday = 0;
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
#pragma mark - GUI
//--------------------------------------------------------------
// *
// * GUI
// *
//--------------------------------------------------------------
void ofApp::setupGUI()
{
    
    title = new ofxDatGuiLabel("Stand and Stare Kiosk");
    title->setPosition(100, 0);
    title->setWidth(200);
    
    appFPS = new ofxDatGuiFRM(1);
    appFPS->setPosition((title->getWidth()+title->getX()), 0);
    appFPS->setWidth(50, 0);
    appFPS->setLabel("");
    appFPS->setStripeVisible(false);
    
    vector<string> AppMode = {"CALIBRATION MODE","ASSIGNING MODE","OPERATION MODE"};
    
    appMode = new ofxDatGuiDropdown("App Mode",AppMode);
    appMode->setWidth(150);
    appMode->expand();
    appMode->setPosition(appFPS->getX()+appFPS->getWidth(),0);
    appMode->onDropdownEvent(this, &ofApp::onDropdownEvent);
    
    showTemplateImage = new ofxDatGuiToggle("Show Template",false);
    showTemplateImage->setWidth(150);
    showTemplateImage->setPosition(appMode->getX()+appMode->getWidth(), 0);
    showTemplateImage->onButtonEvent(this, &ofApp::onButtonEvent);
    
    calibrationFolder = new ofxDatGuiFolder("Calibration");
    calibrationFolder->setWidth(200);
    calibrationFolder->setPosition(showTemplateImage->getX()+showTemplateImage->getWidth(), 0);
    calibrationFolder->addToggle("Enable Mask Creation");
    calibrationFolder->addButton("Clear Mask");
    calibrationFolder->addButton("Save Mask");
    calibrationFolder->addToggle("Show Coordinates");
    calibrationFolder->onButtonEvent(this, &ofApp::onButtonEvent);


    reloadVideoData = new ofxDatGuiButton("Reload Video Data");
    reloadVideoData->setWidth(150);
    reloadVideoData->setPosition(calibrationFolder->getX()+calibrationFolder->getWidth(), 0);
    reloadVideoData->setStripe(ofColor::mediumSpringGreen, 5);
    reloadVideoData->onButtonEvent(this, &ofApp::onButtonEvent);
    
    addNewFile = new ofxDatGuiButton("Add New File");
    addNewFile->setWidth(200);
    addNewFile->setPosition(reloadVideoData->getX(), reloadVideoData->getHeight());
    addNewFile->onButtonEvent(this, &ofApp::onButtonEvent);
    
    videoFolder = new ofxDatGuiFolder("Video");
    videoFolder->setPosition(reloadVideoData->getX()+reloadVideoData->getWidth(),0);
    videoFolder->setWidth(200);
    
    videoFolder->addToggle("Show Primary Warper Quads");
    videoFolder->addToggle("Show Secondary Warper Quads");
    videoFolder->addToggle("Show Quads");
    videoFolder->onButtonEvent(this, &ofApp::onButtonEvent);
    
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
    else if(e.target->is("Show Template")) {
        showTemplate = !showTemplate;
    }
    else if(e.target->is("Show Coordinates")) {
        calibrationScreen.showCoordinates(e.target->getEnabled());
    }
    else if(e.target->is("Enable Mask Creation")) {
        calibrateScreen = e.target->getEnabled();
    }
    else if(e.target->is("Show Secondary Warper Quads")){
        enticer.showSecondaryQuad(e.target->getEnabled());
    }
    else if(e.target->is("Show Primary Warper Quads")) {
        enticer.showPrimaryQuad(e.target->getEnabled());
    }
    else if(e.target->is("Show Warper")) {
        showWarper = e.target->getEnabled();
    }
    else if(e.target->is("Reload Video Data")) {
        appConfiguration.loadVideoConfig("configs/videoConfig.json");
        videoData.clear();
        videoData = appConfiguration.getVideoConfig();
        tagAssigner.reloadData(videoData);
    }
    else if(e.target->is("Add New File")) {
        stopLoad = false;
        tagAssigner.addNewFile();
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