#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::appSetup()
{
    appConfig.load("config.json");
    
    // Setup the SSH Key listener | Thanks @JVCleave
    consoleListener.setup(this);
}
//--------------------------------------------------------------
void ofApp::setup()
{
    appSetup();
    
    // Audio Player
    audioHandler.setupAudioPlayer(appConfig.getConfig().fadeInTime, appConfig.getConfig().fadeOutTime);
    
    // RFID
    rfidReader.setup(appConfig.getConfig().RFIDSerialName,2000);
    
    // This is for Posting Data Back to the server
    postData.setup(appConfig.getConfig().postHostURL, appConfig.getConfig().postExtURL, appConfig.getConfig().secretKey);
    
    // For the Enticing Display
    enticer.setupAudioPlayer(appConfig.getConfig().enticerFadeIn, appConfig.getConfig().enticerFadeOut);
    enticer.loadAudio(appConfig.getConfig().enticerAudioUrl,appConfig.getConfig().enticerAudioLength);
    enticer.playAudio();
}
//--------------------------------------------------------------
void ofApp::update()
{
    enticer.updateAudio();
    audioHandler.updateAudio();
    rfidReader.update();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);
    
    stringstream debugData;
    debugData << "|----------------------------------" << endl;
    debugData << "| Stand and Stare Jukebox" << endl;
    debugData << enticer.getStringStream() << endl;
    debugData << audioHandler.getStringStream() << endl;
    debugData << rfidReader.getDebugString() << endl;
    ofDrawBitmapString(debugData.str(), 10, 10);
}
//--------------------------------------------------------------
void ofApp::exit()
{
    
}
//--------------------------------------------------------------
void ofApp::onCharacterReceived(KeyListenerEventData &e)
{
    keyPressed((int)e.character);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'p':
            enticer.playAudio();
            break;
        case 's':
            enticer.stopAudio();
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
void ofApp::mouseMoved(int x, int y )
{

}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

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