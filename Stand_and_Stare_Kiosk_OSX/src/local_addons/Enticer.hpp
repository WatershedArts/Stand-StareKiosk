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
#include "ofxQuadWarp.h"

class EnticerVisuals {
    public:
        void setupVideoPlayer(float fadein,float fadeout);
        void loadVideo(string url);
        void updateVideo();
        void playVideo();
        void stopVideo();
    
        void drawVideo();
        void drawCalibrationQuads();
    
        float getTimeLeft();
        bool hasVideoFinished();
        bool isVideoPlaying();
        string getStringStream();
    
        void showPrimaryQuad(bool val);
        void showSecondaryQuad(bool val);
    
    private:
        ofVideoPlayer videoPlayer;
        ofxTween fade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
        ofxQuadWarp warper;
        ofFbo warperFbo;
        float _fadein;
        float _fadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;
        bool _drawPrimaryQuads;
        bool _drawSecondaryQuads;
    protected:
        int currentFadeValue;
        float videoLength;
    
};

#endif /* Enticer_hpp */
