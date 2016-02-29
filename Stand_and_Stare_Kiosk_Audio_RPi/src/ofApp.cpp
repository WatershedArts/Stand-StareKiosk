#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::appSetup()
{
    appConfig.load("config.json");
    
    titleFont.load("MuseoSans_500.otf", 30);
    defaultFont.load("MuseoSans_500.otf", 15);
    
    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
    hideDebug = false;
    whatsHappening = "Idling";
    
    ofAddListener(audioHandler.trackStarted, this, &ofApp::trackStarted);
    ofAddListener(audioHandler.trackFinishedNormally, this, &ofApp::trackFinished);
    ofAddListener(audioHandler.trackForceFinished, this, &ofApp::trackInterupted);
    
    ofAddListener(rfidReader.newTag, this, &ofApp::newTagAdded);
    ofAddListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
    
    ofAddListener(enticer.trackStarted, this, &ofApp::enticerTrackStarted);
    ofAddListener(enticer.trackForceFinished, this, &ofApp::enticerTrackFinished);
}
#pragma mark - oF Methods
//--------------------------------------------------------------
// *
// *    ofMethods
// *
//--------------------------------------------------------------
void ofApp::setup()
{
    // Load App Settings
    appSetup();
    
    // Get Audio Track Data
    audioData = appConfig.getConfig().audios;
    
    // Audio Player
    audioHandler.setupAudioPlayer(appConfig.getConfig().fadeInTime,
                                  appConfig.getConfig().fadeOutTime,
                                  appConfig.getConfig().enticerFadeOut
                                  );
    audioHandler.loadAudio(audioData[0].audioUrl,
                           audioData[0].audioLength
                           );
    
    // RFID
    rfidReader.setup(appConfig.getConfig().RFIDSerialName,2000);
    rfidReader.start();

    // This is for Posting Data Back to the server
    postData.setup(appConfig.getConfig().postHostURL,
                   appConfig.getConfig().postExtURL,
                   appConfig.getConfig().secretKey
                   );
    
    // For the Enticing Audio
    enticer.setupAudioPlayer(appConfig.getConfig().enticerFadeIn,
                             appConfig.getConfig().enticerFadeOut,
                             appConfig.getConfig().fadeOutTime
                             );

    enticer.loadAudio(appConfig.getConfig().enticerAudioUrl,
                      appConfig.getConfig().enticerAudioLength
                      );
    
    enticer.playAudio();
}
//--------------------------------------------------------------
void ofApp::update()
{
//    rfidReader.update();
    enticer.updateAudio();
    audioHandler.updateAudio();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(25);

    ofSetColor(ofColor::white);
    stringstream debugData;
    debugData << "|----------------------------------" << endl;
    debugData << "| Stand and Stare Jukebox" << endl;
    debugData << enticer.getStringStream() << endl;
    debugData << audioHandler.getStringStream() << endl;
    debugData << rfidReader.getDebugString() << endl;

    if (hideDebug) {
        ofDrawBitmapString(debugData.str(), 10, 100);
    }
    
    titleFont.drawString("Stand & Stare Kiosk", 10, 50);
    defaultFont.drawString(whatsHappening, 10, 80);
    
    enticer.draw(125);
    audioHandler.draw(200);
    
    drawLabels();
}
//--------------------------------------------------------------
void ofApp::exit()
{
    ofRemoveListener(audioHandler.trackStarted, this, &ofApp::trackStarted);
    ofRemoveListener(audioHandler.trackFinishedNormally, this, &ofApp::trackFinished);
    ofRemoveListener(audioHandler.trackForceFinished, this, &ofApp::trackInterupted);
    
    ofRemoveListener(enticer.trackStarted, this, &ofApp::enticerTrackStarted);
    ofRemoveListener(enticer.trackForceFinished, this, &ofApp::enticerTrackFinished);
    
    ofRemoveListener(rfidReader.newTag, this, &ofApp::newTagAdded);
    ofRemoveListener(rfidReader.tagRemoved, this, &ofApp::tagRemoved);
    rfidReader.stop();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case '1':
            rfidReader.simulateNewTag(1);
            break;
        case '2':
            rfidReader.simulateNewTag(2);
            break;
        case '3':
            rfidReader.simulateNewTag(3);
            break;
        case 'p':
            audioHandler.forceStopAudio();
            enticer.playAudio();
            break;
        case 's':
            enticer.stopAudio();
            break;
        case 'r':
            rfidReader.simulateTagRemoval();
            break;
        case ' ':
            hideDebug = !hideDebug;
            break;
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}
#pragma mark - Listeners
//--------------------------------------------------------------
// *
// *    LISTENERS
// *
//--------------------------------------------------------------
void ofApp::trackStarted(string &args)
{
    whatsHappening = "Playing Track: " + args.substr(29,args.length());
    ofLogNotice() << args;
}
//--------------------------------------------------------------
void ofApp::trackFinished(string &args)
{
    whatsHappening = "Stopping Track: " + args.substr(29,args.length());
    enticer.playAudio();
    ofLogNotice() << args;
}
//--------------------------------------------------------------
void ofApp::trackInterupted(string &args)
{
    whatsHappening = "Track Interupted: " + args.substr(36,args.length());
    enticer.playAudio();
    ofLogNotice() << args;
}
//--------------------------------------------------------------
void ofApp::newTagAdded(string &tag)
{
    ofLogNotice() << tag;
    for (int i = 0; i < audioData.size(); i++) {
        if (tag == audioData[i].RFIDkey) {
            enticer.stopAudio();
            audioHandler.loadAudio(audioData[i].audioUrl, audioData[i].audioLength);
            audioHandler.playAudio();
        }
    }
}
//--------------------------------------------------------------
void ofApp::tagRemoved(string &tag)
{
    ofLogNotice() << tag;
    audioHandler.stopAudio();
    enticer.playAudio();
}
//--------------------------------------------------------------
void ofApp::enticerTrackStarted(string &args)
{
    whatsHappening = "Idling";
    ofLogNotice() << args;
}
//--------------------------------------------------------------
void ofApp::enticerTrackFinished(string &args)
{
    ofLogNotice() << args;
}
//--------------------------------------------------------------
// *
// *
// *
//--------------------------------------------------------------
void ofApp::drawLabels()
{
    int centerX = ofGetWidth()*0.5;
    int centerY = ofGetHeight()*0.90;
    
    ofRectangle info = defaultFont.getStringBoundingBox("Current RFID Tag:", 0, 0);
    ofRectangle currentTagBox = defaultFont.getStringBoundingBox(rfidReader.getCurrentTag(), 0, 0);
    
    int newTagCenterX = centerX - (currentTagBox.width/2);
    int newTagCenterY = centerY - (currentTagBox.height/2);
    
    int newInfoCenterX = centerX - (info.width/2);
    
    defaultFont.drawString("Current RFID Tag:",newInfoCenterX,newTagCenterY-currentTagBox.height-10);
    defaultFont.drawString(rfidReader.getCurrentTag(),newTagCenterX,newTagCenterY);
}
//--------------------------------------------------------------
void ofApp::onCharacterReceived(KeyListenerEventData &e)
{
    keyPressed((int)e.character);
}
