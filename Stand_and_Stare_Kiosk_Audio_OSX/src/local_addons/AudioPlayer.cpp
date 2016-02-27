//
//  AudioPlayer.cpp
//  Stand_and_Stare_Kiosk_Audio_OSX
//
//  Created by David Haylock on 27/02/2016.
//
//

#include "AudioPlayer.hpp"

//--------------------------------------------------------------
void AudioPlayer::setupAudioPlayer(float fadein, float fadeout)
{
    _fadein = fadein;
    _fadeout = fadeout;
    _hasFadedOut = true;
    _hasFadedIn = true;
}
//--------------------------------------------------------------
void AudioPlayer::loadAudio(string url,float _audioLength)
{
    audioPlayer.load(url);
    audioLength = _audioLength;
    cout << "Loading " << url << endl;
}
//--------------------------------------------------------------
void AudioPlayer::updateAudio()
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
void AudioPlayer::playAudio()
{
    audioPlayer.play();
    cout << "Playing Video" << endl;
    fade.setParameters(1, easinglinear, ofxTween::easeIn, 0.000, 1.000, _fadein, 2000);
    _hasFadedIn = false;
}
//--------------------------------------------------------------
void AudioPlayer::stopAudio()
{
    fade.setParameters(1, easingexpo, ofxTween::easeOut, currentFadeValue, 0.000, _fadeout, 10);
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