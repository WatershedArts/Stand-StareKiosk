//
//  Enticer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "Enticer.hpp"
//--------------------------------------------------------------
void EnticerVisuals::setupVideoPlayer(float fadein, float fadeout,float videoHandlerFadeOut)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _vHFadeout = videoHandlerFadeOut;
    _drawPrimaryQuads = false;
    _drawSecondaryQuads = false;
    warper.disableKeyboardShortcuts();
}
//--------------------------------------------------------------
void EnticerVisuals::loadVideo(string url)
{
    videoPlayer.load(url);
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    videoName = url;
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
            string ev =  "Enticer Stopped";
            ofNotifyEvent(enticerInterrupted, ev, this);
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
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, _vHFadeout);
    string ev =  "Enticer Started";
    ofNotifyEvent(enticerStarted, ev, this);
    _hasFadedIn = false;
}
//--------------------------------------------------------------
void EnticerVisuals::stopVideo()
{
    fade.setParameters(1, easinglinear, ofxTween::easeInOut, currentFadeValue, 0, _fadeout, 0);
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void EnticerVisuals::drawVideo()
{
    if(videoPlayer.isPlaying()) {
        ofPushMatrix();
        ofPushStyle();
        ofMatrix4x4 mat = warper.getMatrix();
        ofPushMatrix();
        ofSetColor(currentFadeValue);
        ofMultMatrix(mat);
        warperFbo.draw(0, 0);
        ofPopMatrix();
        ofPopStyle();
        ofPopMatrix();
    }
}
//--------------------------------------------------------------
void EnticerVisuals::drawTimeline(int y)
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


    ofPushMatrix();
    ofTranslate(0, y);
    ofPushStyle();
    ofFill();
    ofSetColor(50, 50, 50);
    ofDrawRectRounded(startX,0, playBarLength, 25, 5);
    ofSetColor(ofMap(fade.update(), 0.00, 255, 255, 0), fade.update(), 0);
    ofDrawRectRounded(startX,0, progress-startX, 25, 5);

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
//    if (_hasFadedOut && (fade.isCompleted() || fade.isRunning())) {
        ofSetColor(ofColor::white);
        ofDrawLine(progress, -15, progress, 25);
        ofDrawBitmapString(trackTimeElapsed,progress+10 ,-5);
//    }
//    else {
//        ofSetColor(ofColor::white);
//        ofDrawLine(dropFade.update()+offset, -15, dropFade.update()+offset, 25);
//        ofDrawBitmapString(trackTimeElapsed,dropFade.update()+offset+10 ,-5);
//    }

    string isAudioPlayingStr = isVideoPlaying() ? "Playing: " : "Stopped: ";
    ofDrawBitmapString(isAudioPlayingStr + videoName.substr(7,videoName.length()), offset, 40);
    ofPopMatrix();
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
    datastream << "|----------------------------------" << endl;
    datastream << "| Enticer Player" << endl;
    datastream << "|----------------------------------" << endl;
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