//
//  Enticer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "Enticer.hpp"
//--------------------------------------------------------------
void EnticerVisuals::setupVideoPlayer(float fadein, float fadeout)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _drawPrimaryQuads = false;
    _drawSecondaryQuads = false;
    warper.disableKeyboardShortcuts();
}
//--------------------------------------------------------------
void EnticerVisuals::loadVideo(string url)
{
    videoPlayer.load(url);
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    videoLength = videoPlayer.getDuration();
    cout << "Loading " << url << endl;
    int x = (ofGetWidth() - videoPlayer.getWidth()) * 0.5;
    int y = (ofGetHeight() - videoPlayer.getHeight()) * 0.5;
    int w = videoPlayer.getWidth();
    int h = videoPlayer.getHeight();
    
    warperFbo.allocate(w, h);
    warper.setSourceRect(ofRectangle(0, 0, w, h));
    warper.setTopLeftCornerPosition(ofPoint(x, y));
    warper.setTopRightCornerPosition(ofPoint(x + w, y));
    warper.setBottomLeftCornerPosition(ofPoint(x, y + h));
    warper.setBottomRightCornerPosition(ofPoint(x + w, y + h));
    warper.setup();
}
//--------------------------------------------------------------
void EnticerVisuals::updateVideo()
{
    videoPlayer.update();
    currentFadeValue = fade.update();
    videoLength = videoPlayer.getDuration();
    
    if (fade.isCompleted() && !_hasFadedOut) {
        videoPlayer.stop();
        if (!videoPlayer.isPlaying()) {
            cout << "Stopped Video" << endl;
        }
        _hasFadedOut = true;
    }
    
    float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
    if (currentVideoTime >= (videoLength-(_fadeout/1000))) {
        if (_hasFadedOut) {
            stopVideo();
        }
    }
    
    
    warperFbo.begin();
    ofSetColor(fade.update(),255);
    if (videoPlayer.isLoaded()) {
        if(videoPlayer.isPlaying()) {
            videoPlayer.draw(0, 0);
        }
    }
    warperFbo.end();

}
//--------------------------------------------------------------
void EnticerVisuals::drawCalibrationQuads()
{
    ofPushStyle();
    if (_drawPrimaryQuads) {
        warper.enableMouseControls();
        ofSetColor(ofColor::blue);
        warper.drawQuadOutline();
        
        ofSetColor(ofColor::greenYellow);
        warper.drawCorners();
        
        ofSetColor(ofColor::red);
        warper.drawHighlightedCorner();
        
        ofSetColor(ofColor::pink);
        warper.drawSelectedCorner();
    }
    else {
        warper.disableMouseControls();
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void EnticerVisuals::playVideo()
{
    videoPlayer.play();
    cout << "Playing Video" << endl;
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, 2000);
    _hasFadedIn = false;
}
//--------------------------------------------------------------
void EnticerVisuals::stopVideo()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
    _hasFadedOut = false;
//    ofMessage msg("Natural Stop");
//    ofSendMessage(msg);
}
//--------------------------------------------------------------
void EnticerVisuals::drawVideo()
{
    if(videoPlayer.isPlaying()) {
        ofPushMatrix();
        ofPushStyle();
        ofMatrix4x4 mat = warper.getMatrix();
        ofPushMatrix();
        ofSetColor(ofColor::white);
        ofMultMatrix(mat);
        warperFbo.draw(0, 0);
        ofPopMatrix();
        ofPopStyle();
        ofPopMatrix();
    }
}
//--------------------------------------------------------------
void EnticerVisuals::showPrimaryQuad(bool val)
{
    _drawPrimaryQuads = val;
}
//--------------------------------------------------------------
void EnticerVisuals::showSecondaryQuad(bool val)
{
    _drawSecondaryQuads = val;
}
//--------------------------------------------------------------
string EnticerVisuals::getStringStream()
{
    stringstream datastream;
    float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
    string thisstring = videoPlayer.isPlaying() ? "Currently Playing" : "Standing By";
    datastream << "| Video is " << thisstring << endl;
    datastream << "| Current Time: " << currentVideoTime << endl;
    datastream << "| Video Position: " << videoPlayer.getPosition() << endl;
    datastream << "| Fade Out Time: " << (float)(videoLength-(_fadeout/1000)) << endl;
    datastream << "| Video Duration: " << (float)videoPlayer.getDuration() << endl;
    return datastream.str();
}
//--------------------------------------------------------------
float EnticerVisuals::getTimeLeft()
{
    return videoPlayer.getPosition();
}
//--------------------------------------------------------------
bool EnticerVisuals::isVideoPlaying()
{
    return videoPlayer.isPlaying();
}
//--------------------------------------------------------------
bool EnticerVisuals::hasVideoFinished()
{
    return _hasFadedOut;
}