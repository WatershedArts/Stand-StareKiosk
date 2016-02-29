//
//  AudioPlayer.hpp
//  Stand_and_Stare_Kiosk_Audio_OSX
//
//  Created by David Haylock on 27/02/2016.
//
//

#ifndef AudioPlayer_hpp
#define AudioPlayer_hpp

#include <stdio.h>
#include <stdio.h>
#include "ofMain.h"
#include "ofxTween.h"

class AudioPlayer {
public:
    void setupAudioPlayer(float fadein,float fadeout,float delayTime);
    void loadAudio(string url,float audioLength);
    void updateAudio();
    
    void playAudio();
    void stopAudio();
    void forceStopAudio();
    
    void draw(int y);
    
    float getTimeLeft();
    bool hasAudioFinished();
    bool isAudioPlaying();
    
    string getStringStream();
    ofEvent<string> trackStarted;
    ofEvent<string> trackFinishedNormally;
    ofEvent<string> trackForceFinished;
    
private:
    ofSoundPlayer audioPlayer;
    
    ofxTween fade;
    ofxEasingLinear easinglinear;
    ofxEasingExpo easingexpo;

    ofxTween dropFade;
    
    
    int progress;
    float _fadein;
    float _fadeout;
    bool _hasFadedIn;
    bool _hasFadedOut;
    bool _delayTime;
    string trackName;
    
protected:
    float currentFadeValue;
    float audioLength;
    
};
#endif /* AudioPlayer_hpp */
