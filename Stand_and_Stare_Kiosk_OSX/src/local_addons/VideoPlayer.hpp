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

class VideoPlayer {
    
    public:
        //! Setup Video Handler
        void setupVideoPlayer(float fadein,float fadeout,float enticerFadeIn);
    
        //! Load the Video
        void loadVideo(string url);
    
        //! Update video
        void updateVideo();
    
        //! Play Video
        void playVideo();
    
        //! Stop Video
        void stopVideo();
    
        //! Interrupt the Video
        void interruptVideo();

        //! Draw
        void drawVideo();
    
        //! Draw Timeline
        void drawTimeline(int y);
    
        //! Draw the Warpers
        void drawCalibrationQuads();
    
        //! Show the Unwarped Quads
        void showPrimaryQuad(bool val);
    
        //! Show the Warped Quads
        void showSecondaryQuad(bool val);
    
        //! How many Seconds Left
        float getTimeLeft();
    
        //! Has the Video Finished
        bool hasVideoFinished();
    
        //! Is the Video Playing
        bool isVideoPlaying();
    
        //! Debug Info
        string getStringStream();
    
        //! What Percentage through the Video are We?
        int getPlayPercentage();
    
        // Events
        ofEvent<string> videoStarted;
        ofEvent<string> videoStopped;
        ofEvent<string> videoInterrupted;
    
    private:
        ofVideoPlayer videoPlayer;
        ofxTween fade;
        ofxTween dropFade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
//        ofTimer delayTimer;
    
        ofxQuadWarp warper;
        ofFbo warperFbo;
        vector <string> files;
        float _fadein;
        float _fadeout;
        float _enticerDelay;
        int progress;
        bool _hasFadedIn;
        bool _hasFadedOut;
        bool _drawPrimaryQuads;
        bool _drawSecondaryQuads;
        string videoName;

    protected:
        int currentFadeValue;
        float videoLength;
};

#endif /* VideoPlayer_hpp */
