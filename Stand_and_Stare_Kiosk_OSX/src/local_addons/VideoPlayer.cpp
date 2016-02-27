//
//  VideoPlayer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "VideoPlayer.hpp"

//--------------------------------------------------------------
void VideoPlayer::setupVideoPlayer(float fadein, float fadeout)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _drawPrimaryQuads = false;
    _drawSecondaryQuads = false;
    
    warper.disableKeyboardShortcuts();
    
    ofDirectory videoDirectory(ofToDataPath("videos",true));
    if (videoDirectory.exists()) {
        cout << "VideoDirectory: Found" << endl;
        for(int file = 0; file < videoDirectory.getFiles().size(); file++) {
            files.push_back(videoDirectory.getFile(file).path());
        }
    }
    else{
        ofLogError() << "VideoDirectory: " << videoDirectory.path() << " MISSING";
    }
}
//--------------------------------------------------------------
void VideoPlayer::loadVideo(string url)
{
    videoPlayer.load(url);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoLength = videoPlayer.getDuration();
    
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
void VideoPlayer::updateVideo()
{
    videoPlayer.update();
    currentFadeValue = fade.update();
    videoLength = videoPlayer.getDuration();
    float volumeMap = ofMap(fade.update(), 0, 255, 0.00, 1.00);
    videoPlayer.setVolume(volumeMap);
    
    if (fade.isCompleted() && !_hasFadedOut) {
        videoPlayer.stop();
        if (!videoPlayer.isPlaying()) {
            cout << "Stopped Video" << endl;
        }
        _hasFadedOut = true;
    }
    
    float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
    if (currentVideoTime >= (videoLength-(_fadeout/1000))) {
        if (_hasFadedOut && isVideoPlaying()) {
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
void VideoPlayer::playVideo()
{
    videoPlayer.play();
    cout << "Playing Video" << endl;
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, 1000);
   _hasFadedIn = false;
}
//--------------------------------------------------------------
void VideoPlayer::stopVideo()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
    _hasFadedOut = false;
    ofMessage msg("Natural Stop");
    ofSendMessage(msg);
}
//--------------------------------------------------------------
void VideoPlayer::stopVideoPlus()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
    _hasFadedOut = false;
    ofMessage msg("Forced Stop");
    ofSendMessage(msg);
}
//--------------------------------------------------------------
void VideoPlayer::showPrimaryQuad(bool val)
{
    _drawPrimaryQuads = val;
}
//--------------------------------------------------------------
void VideoPlayer::showSecondaryQuad(bool val)
{
    _drawSecondaryQuads = val;
}
//--------------------------------------------------------------
void VideoPlayer::drawCalibrationQuads()
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
void VideoPlayer::drawVideo()
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
    
    
    if (_drawSecondaryQuads) {
        warper.enableMouseControls();
        ofPushStyle();
        ofSetLineWidth(3);
        ofSetColor(ofColor::white);
        warper.drawQuadOutline();
        ofPopStyle();
        
        ofSetColor(ofColor::yellow);
        warper.drawCorners();
        
        ofSetColor(ofColor::magenta);
        warper.drawHighlightedCorner();
        
        ofSetColor(ofColor::red);
        warper.drawSelectedCorner();
    }
    else {
        warper.disableMouseControls();
    }
}
//--------------------------------------------------------------
string VideoPlayer::getStringStream()
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
int VideoPlayer::getPlayPercentage()
{
    return videoPlayer.getPosition()*100;
}
//--------------------------------------------------------------
float VideoPlayer::getTimeLeft()
{
    return videoPlayer.getPosition();
}
//--------------------------------------------------------------
bool VideoPlayer::isVideoPlaying()
{
    return videoPlayer.isPlaying();
}
//--------------------------------------------------------------
bool VideoPlayer::hasVideoFinished()
{
    return _hasFadedOut;
}