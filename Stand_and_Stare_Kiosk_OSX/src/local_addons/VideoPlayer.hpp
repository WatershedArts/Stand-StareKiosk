//
//  VideoPlayer.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef VideoPlayer_hpp
#define VideoPlayer_hpp

#include <stdio.h>
#include "ofxTween.h"
#include "ofxQuadWarp.h"
//#include "VideoTimerVisualisation.h"

class VideoPlayer {
    
    public:
        void setupVideoPlayer(float fadein,float fadeout);
        void loadVideo(string url);
        void updateVideo();
        void playVideo();
        void stopVideo();
        void stopVideoPlus();
        void drawVideo();
        void drawCalibrationQuads();
        void showPrimaryQuad(bool val);
        void showSecondaryQuad(bool val);
        float getTimeLeft();
        bool hasVideoFinished();
        bool isVideoPlaying();
        string getStringStream();
        int getPlayPercentage();
    
    private:
        ofVideoPlayer videoPlayer;
        ofxTween fade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
        ofxQuadWarp warper;
        ofFbo warperFbo;
        vector <string> files;
        float _fadein;
        float _fadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;
        bool _drawPrimaryQuads;
        bool _drawSecondaryQuads;
    protected:
        int currentFadeValue;
        float videoLength;
//        VideoTimerVisualisation timerVisualisation;
    
};

#endif /* VideoPlayer_hpp */
