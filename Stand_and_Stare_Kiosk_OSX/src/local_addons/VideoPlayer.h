//
//  VideoPlayer.h
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef VideoPlayer_h
#define VideoPlayer_h

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
    
        //! How many Seconds Left
        float getTimeLeft();
    
        //! Has the Video Finished
        bool hasVideoFinished();

        //! Is the Video Playing
        bool isVideoPlaying();
    
        //! Debug Info
        string getStringStream();
    
        //! Invalidate The fading system
        void invalidateFade();

        //! What Percentage through the Video are We?
        int getPlayPercentage();
    
        void fadingResults(string &str);
    
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
    
        vector <string> files;
        float _fadein;
        float _fadeout;
        float _enticerDelay;
        int progress;
        bool _hasFadedIn;
        bool _hasFadedOut;
    
        bool _isFadingIn;
        bool _isFadingOut;
    
        bool _drawPrimaryQuads;
        bool _drawSecondaryQuads;
        string videoName;
    
        bool goToYellowScreen;

    protected:
        int currentFadeValue;
        float videoLength;
};

#endif /* VideoPlayer_h */
