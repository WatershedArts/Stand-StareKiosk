//
//  AudioPlayer.cpp
//  Stand_and_Stare_Kiosk_Audio_OSX
//
//  Created by David Haylock on 27/02/2016.
//
//

#include "AudioPlayer.hpp"

//--------------------------------------------------------------
void AudioPlayer::setupAudioPlayer(float fadein, float fadeout,float delayTime)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
    _delayTime = delayTime;
}
//--------------------------------------------------------------
void AudioPlayer::loadAudio(string url,float _audioLength)
{
    audioPlayer.unload();
    trackName = url;
    _hasFadedOut = true;
    _hasFadedIn = true;
    audioPlayer.load(url);
    audioLength = _audioLength;
}
//--------------------------------------------------------------
void AudioPlayer::updateAudio()
{
//    ofSoundUpdate();
    currentFadeValue = fade.update();
    audioPlayer.setVolume(currentFadeValue);
    
    if (fade.isCompleted() && !_hasFadedOut) {
        audioPlayer.stop();
        if (!audioPlayer.isPlaying()) { }
        _hasFadedOut = true;
    }
    
    float currentAudioTime = (float)(audioPlayer.getPosition()*audioLength);
    if (currentAudioTime >= (audioLength-(_fadeout/1000))) {
        if (_hasFadedOut) {
            stopAudio();
        }
    }
}
//--------------------------------------------------------------
void AudioPlayer::draw(int y)
{
    int offset = 50;
    int ticks = 20;
    int playBarLength = ofGetWidth()-(offset*2);
    int tickOffset = playBarLength/ticks;
    int centerX = ofGetWidth()*0.5;
    int centerY = ofGetHeight()*0.5;
    int startX = centerX - playBarLength/2;
    
    progress = startX+ofMap(audioPlayer.getPosition(),0.00,1.00,0,playBarLength);
    
    ofPushMatrix();
    ofTranslate(0, y);
    ofPushStyle();
    ofFill();
    ofSetColor(50, 50, 50);
    ofDrawRectRounded(startX,0, playBarLength, 25, 5);
    ofSetColor(ofMap(fade.update(), 0.00, 1.00, 255, 0), ofMap(fade.update(), 0.00, 1.00, 0, 255), 0);
    
    if (_hasFadedOut && (fade.isCompleted() || fade.isRunning())) {
        ofDrawRectRounded(startX,0, progress-startX, 25, 5);
    }
    else {
        ofDrawRectRounded(startX,0, dropFade.update(), 25, 5);
    }
    
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
    
    int posMS = audioPlayer.getPositionMS();
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
    
    
    if (_hasFadedOut && (fade.isCompleted() || fade.isRunning())) {
        ofSetColor(ofColor::white);
        ofDrawLine(progress, -15, progress, 25);
    }
    else {
        ofSetColor(ofColor::white);
        ofDrawLine(dropFade.update()+offset, -15, dropFade.update()+offset, 25);
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
    
    string isAudioPlayingStr = isAudioPlaying() ? "Playing: " : "Stopped: ";
    ofDrawBitmapString(isAudioPlayingStr + trackName.substr(6,trackName.length()), offset, 40);
    ofPopMatrix();
}
//--------------------------------------------------------------
void AudioPlayer::playAudio()
{
    audioPlayer.play();
    string ev =  "Audio Handler Started: " + trackName;
    ofNotifyEvent(trackStarted, ev , this);
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0.000, 1.000, _fadein, _delayTime);
    _hasFadedIn = false;
}
//--------------------------------------------------------------
void AudioPlayer::stopAudio()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0.000, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev =  "Audio Handler Stopped: " + trackName;
    ofNotifyEvent(trackFinishedNormally, ev , this);
    audioPlayer.unload();
    _hasFadedOut = false;
}
//--------------------------------------------------------------
void AudioPlayer::forceStopAudio()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0.000, _fadeout, 10);
    dropFade.setParameters(1, easingexpo, ofxTween::easeOut, progress-50, 0, _fadeout, 10);
    string ev =  "Audio Handler Force Stopped: " + trackName;
    ofNotifyEvent(trackForceFinished, ev , this);
    audioPlayer.unload();
    _hasFadedOut = false;
}
//--------------------------------------------------------------
string AudioPlayer::getStringStream()
{
    stringstream datastream;
    float currentAudioTime = (float)(audioPlayer.getPosition()*audioLength);
    string thisstring = audioPlayer.isPlaying() ? "Currently Playing" : "Standing By";
    datastream << "|----------------------------------" << endl;
    datastream << "| Audio Player" << endl;
    datastream << "|----------------------------------" << endl;
    datastream << "| Audio is " << thisstring << endl;
    datastream << "| Current Time: " << currentAudioTime << endl;
    datastream << "| Audio Position: " << audioPlayer.getPosition() << endl;
    datastream << "| Fade Out Time: " << (float)(audioLength-(_fadeout/1000)) << endl;
    datastream << "| Audio Duration: " << (float)audioLength << endl;
    return datastream.str();
}
//--------------------------------------------------------------
float AudioPlayer::getTimeLeft()
{
    return audioPlayer.getPosition();
}
//--------------------------------------------------------------
bool AudioPlayer::isAudioPlaying()
{
    return audioPlayer.isPlaying();
}
//--------------------------------------------------------------
bool AudioPlayer::hasAudioFinished()
{
    return _hasFadedOut;
}
