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
        void setupAudioPlayer(float fadein,float fadeout);
        void loadAudio(string url,float audioLength);
        void updateAudio();
    
        void playAudio();
        void stopAudio();
    
        float getTimeLeft();
        bool hasAudioFinished();
        bool isAudioPlaying();
    
        string getStringStream();
    
    private:
        ofSoundPlayer audioPlayer;
    
        ofxTween fade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
        float _fadein;
        float _fadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;

    protected:
        float currentFadeValue;
        float audioLength;
    
};

#endif /* Enticer_hpp */
