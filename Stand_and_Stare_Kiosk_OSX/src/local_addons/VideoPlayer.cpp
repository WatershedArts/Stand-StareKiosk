//
//  VideoPlayer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "VideoPlayer.h"

//--------------------------------------------------------------
void VideoPlayer::setupVideoPlayer(float fadein, float fadeout,float enticerFadeIn)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _isFadingIn = false;
    _isFadingOut = false;
    _drawPrimaryQuads = false;
    _drawSecondaryQuads = false;
    goToYellowScreen = false;
    _enticerDelay = enticerFadeIn;
    
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
    videoName = url;
//    cout << videoName << endl;
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoLength = videoPlayer.getDuration();
    
    int x = 0;
    int y = 0;
    int w = videoPlayer.getWidth();
    int h = videoPlayer.getHeight();
    
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
            crossFadeAndStop();
        }
    }
}
//--------------------------------------------------------------
void VideoPlayer::playVideo()
{
    goToYellowScreen = false;
    videoPlayer.play();
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, _enticerDelay);
    string ev = "Video Started";
    ofNotifyEvent(videoStarted, ev, this);
   _hasFadedIn = false;
}
//--------------------------------------------------------------
void VideoPlayer::stopVideo()
{
    fade.setParameters(1, easinglinear, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev = "Video Stopped";
    ofNotifyEvent(videoStopped, ev, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::crossFadeAndStop()
{
    fade.setParameters(1, easinglinear, ofxTween::easeOut, currentFadeValue, 0, _fadeout, _enticerDelay/2);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev = "Video Stopped";
    ofNotifyEvent(videoStopped, ev, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::interruptVideo()
{
    fade.setParameters(1, easinglinear, ofxTween::easeInOut, currentFadeValue, 0, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev = "Video Interrupted";
    ofNotifyEvent(videoInterrupted, ev, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::invalidateFade()
{
    fade.setParameters(1, easinglinear, ofxTween::easeInOut, currentFadeValue, 255, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev = "Invalidate Fade";
    ofNotifyEvent(videoInterrupted, ev, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::drawVideo()
{
    ofPushStyle();
    ofSetColor(fade.update(),255);
    if (videoPlayer.isLoaded()) {
        if(videoPlayer.isPlaying()) {
            videoPlayer.draw(0, 0);
        }
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void VideoPlayer::drawTimeline(int y)
{
    int offset = 350;
    int ticks = 20;
    int playBarLength = ofGetWidth()-(offset*2);
    int tickOffset = playBarLength/ticks;
    int centerX = ofGetWidth()*0.5;
    int centerY = ofGetHeight()*0.5;
    int startX = centerX - playBarLength/2;
    
    if (!videoPlayer.isPlaying()) {
        progress = startX;
    }
    else {
        progress = startX+ofMap(videoPlayer.getPosition(),0.00,1.00,0,playBarLength);
    }
    ofClamp(progress, 0, startX+playBarLength);
    
    ofPushMatrix();
    ofTranslate(0, y);
    ofPushStyle();
    ofFill();
    ofSetColor(50, 50, 50);
    ofDrawRectRounded(startX,0, playBarLength, 25, 5);
    ofSetColor(ofMap(fade.update(), 0.00, 255, 255, 0), fade.update(), 0);

    if (_hasFadedOut && (fade.isCompleted() || fade.isRunning())) {
        ofDrawRectRounded(startX,0, progress-startX, 25, 5);
    }
    else {
        ofDrawRectRounded(startX,0, dropFade.update(), 25, 5);
    }
    //ofDrawRectRounded(startX,0, progress-startX, 25, 5);
    
    ofPushStyle();
    ofSetLineWidth(3);
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawRectRounded(startX,0, playBarLength, 25, 5);
    ofSetLineWidth(1);
    for (int x = 0;  x < ticks; x++) {
        if (x % 2 == 0) {
            ofDrawLine(startX+(x*tickOffset), 7, startX+(x*tickOffset), 25);
        }
        else {
            ofDrawLine(startX+(x*tickOffset), 15, startX+(x*tickOffset), 25);
        }
    }
    
    ofPopStyle();
    ofPopStyle();
    ofSetLineWidth(2);
    
    int posSEC = videoPlayer.getPosition()*videoLength;
    int posMS = posSEC * 1000;
    int minutes = (posMS % (1000*60*60)) / (1000*60);
    int seconds = ((posMS % (1000*60*60)) % (1000*60)) / 1000;
    
    string secs;
    if (seconds < 10) {
        secs = "0"+ofToString(seconds);
    }
    else {
        secs = ofToString(seconds);
    }
    
    string mins;
    if (minutes < 10) {
        mins = "0"+ofToString(minutes);
    }
    else {
        mins = ofToString(minutes);
    }
    
    string trackTimeElapsed = mins + ":"+secs;
    if (_hasFadedOut && (fade.isCompleted() || fade.isRunning())) {
        ofSetColor(ofColor::white);
        ofDrawLine(progress, -15, progress, 25);
        ofDrawBitmapString(trackTimeElapsed,progress+10 ,-5);
    }
    else {
        ofSetColor(ofColor::white);
        ofDrawLine(dropFade.update()+offset, -15, dropFade.update()+offset, 25);
        ofDrawBitmapString(trackTimeElapsed,dropFade.update()+offset+10 ,-5);
    }
    
    string isAudioPlayingStr = isVideoPlaying() ? "Playing: " : "Stopped: ";
    if (videoName.length() == 0) {
        ofDrawBitmapString(isAudioPlayingStr + "No Video", offset, 40);
    }
    else {
        ofDrawBitmapString(isAudioPlayingStr + videoName.substr(7,videoName.length()), offset, 40);
    }
    
    ofPopMatrix();
}
//--------------------------------------------------------------
string VideoPlayer::getStringStream()
{
    stringstream datastream;
    float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
    string thisstring = videoPlayer.isPlaying() ? "Currently Playing" : "Standing By";
    datastream << "|----------------------------------" << endl;
    datastream << "| Video Handler Player" << endl;
    datastream << "|----------------------------------" << endl;
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
    return videoPlayer.getPosition() * 100;
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
//--------------------------------------------------------------
void VideoPlayer::fadingResults(string &str)
{
    
    
    
}