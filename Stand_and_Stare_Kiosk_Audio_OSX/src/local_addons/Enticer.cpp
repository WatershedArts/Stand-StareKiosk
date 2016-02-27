//
//  Enticer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "Enticer.hpp"
//--------------------------------------------------------------
void EnticerVisuals::setupAudioPlayer(float fadein, float fadeout)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
}
//--------------------------------------------------------------
void EnticerVisuals::loadAudio(string url,float _audioLength)
{
    audioPlayer.load(url);
    audioLength = _audioLength;
    cout << "Loading " << url << endl;
    audioPlayer.setLoop(true);
}
//--------------------------------------------------------------
void EnticerVisuals::updateAudio()
{
    ofSoundUpdate();
    currentFadeValue = fade.update();
    audioPlayer.setVolume(currentFadeValue);
    
    if (fade.isCompleted() && !_hasFadedOut) {
        audioPlayer.stop();
        if (!audioPlayer.isPlaying()) {
            cout << "Stopped Audio" << endl;
        }
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
void EnticerVisuals::playAudio()
{
    audioPlayer.play();
    cout << "Playing Video" << endl;
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0.000, 1.000, _fadein, 2000);
    _hasFadedIn = false;
}
//--------------------------------------------------------------
void EnticerVisuals::stopAudio()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0.000, _fadeout, 10);
    _hasFadedOut = false;
    ofMessage msg("Natural Stop");
    ofSendMessage(msg);
}
//--------------------------------------------------------------
string EnticerVisuals::getStringStream()
{
    stringstream datastream;
    float currentAudioTime = (float)(audioPlayer.getPosition()*audioLength);
    string thisstring = audioPlayer.isPlaying() ? "Currently Playing" : "Standing By";
    datastream << "|----------------------------------" << endl;
    datastream << "| Default Audio" << endl;
    datastream << "|----------------------------------" << endl;
    datastream << "| Audio is " << thisstring << endl;
    datastream << "| Current Time: " << currentAudioTime << endl;
    datastream << "| Audio Position: " << audioPlayer.getPosition() << endl;
    datastream << "| Fade Out Time: " << (float)(audioLength-(_fadeout/1000)) << endl;
    datastream << "| Audio Duration: " << (float)audioLength << endl;
    return datastream.str();
}
//--------------------------------------------------------------
float EnticerVisuals::getTimeLeft()
{
    return audioPlayer.getPosition();
}
//--------------------------------------------------------------
bool EnticerVisuals::isAudioPlaying()
{
    return audioPlayer.isPlaying();
}
//--------------------------------------------------------------
bool EnticerVisuals::hasAudioFinished()
{
    return _hasFadedOut;
}