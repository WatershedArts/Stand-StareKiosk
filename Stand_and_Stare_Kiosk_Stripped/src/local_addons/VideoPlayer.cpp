//
//  VideoPlayer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "VideoPlayer.hpp"

//--------------------------------------------------------------
void VideoPlayer::setupVideoPlayer(float fadein, float fadeout,float enticerFadeIn,string enticerUrl)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _enticerDelay = enticerFadeIn;
    _enticerUrl = enticerUrl;
    isEnticer = true;
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
////--------------------------------------------------------------
//void VideoPlayer::start() {
//    startThread();
//}
////--------------------------------------------------------------
//void VideoPlayer::stop()
//{
//    stopThread();
//}
//--------------------------------------------------------------
void VideoPlayer::loadVideo(string url,bool enticer)
{
    if (enticer) {
        videoPlayer.loadAsync(_enticerUrl);
        videoName = "Enticer";
        isEnticer = true;
    }
    else {
//        videoPlayer.load(url);
        videoName = url;
        videoPlayer.loadAsync(url);
        isEnticer = false;
    }
    
    videoLength = videoPlayer.get
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
        }
        _hasFadedOut = true;
    }
    
    float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
    if (currentVideoTime >= (videoLength-(_fadeout/1000))) {
        if (_hasFadedOut && isVideoPlaying()) {
            stopVideo();
        }
    }
}
//--------------------------------------------------------------
void VideoPlayer::playVideo()
{
    videoPlayer.play();
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, _enticerDelay);
//    string ev = "Video Started";
    ofNotifyEvent(videoStarted, videoName, this);
   _hasFadedIn = false;
}
//--------------------------------------------------------------
void VideoPlayer::stopVideo()
{
    fade.setParameters(1, easinglinear, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
//    string ev = "Video Stopped";
    ofNotifyEvent(videoStopped, videoName, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::interruptVideo()
{
    fade.setParameters(1, easinglinear, ofxTween::easeInOut, currentFadeValue, 0, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
//    string ev = "Video Interrupted";
    ofNotifyEvent(videoInterrupted, videoName, this);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void VideoPlayer::setDrawOutline(bool drawOutline)
{
    _drawOutline = drawOutline;
}
//--------------------------------------------------------------
void VideoPlayer::drawVideo()
{
    int x = (ofGetWidth() - videoPlayer.getWidth()) * 0.5;
    int y = (ofGetHeight() - videoPlayer.getHeight()) * 0.5;
    ofPushStyle();
    ofSetColor(fade.update(),255);
    if (videoPlayer.isLoaded()) {
        if(videoPlayer.isPlaying()) {
            videoPlayer.draw(0, 0,ofGetWidth(),ofGetHeight());
        }
    }
    if (_drawOutline) {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(ofColor::whiteSmoke);
//        ofDrawRectangle(0,0,videoPlayer.getWidth(),videoPlayer.getHeight());
        ofPopStyle();
    }
    
    ofPopStyle();
}
//--------------------------------------------------------------
void VideoPlayer::drawTimeline(int y)
{
    int offset = 50;
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
bool VideoPlayer::isEnticerPlaying()
{
    if (isVideoPlaying() && isEnticer) return true;
    return false;
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