//
//  Enticer.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef Enticer_hpp
#define Enticer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxTween.h"

class EnticerVisuals {
    public:
        void setupAudioPlayer(float fadein,float fadeout,float delayTime);
        void loadAudio(string url,float audioLength);
        void updateAudio();
    
        void playAudio();
        void stopAudio();
        void draw(int y);
    
        float getTimeLeft();
        bool hasAudioFinished();
        bool isAudioPlaying();
    
        string getStringStream();
        ofEvent<string> trackStarted;
        ofEvent<string> trackForceFinished;
    
    private:
        ofSoundPlayer audioPlayer;
        ofxTween fade;
        ofxTween dropFade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
        int progress;
        float _fadein;
        float _fadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;
        string trackName;
        float _delayTime;

    protected:
        float currentFadeValue;
        float audioLength;
    
};

#endif /* Enticer_hpp */
