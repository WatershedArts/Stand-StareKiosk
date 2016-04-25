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
#include "ofRPIVideoPlayer.h"

class VideoPlayer : public ofThread  {
    
    public:
        //! Setup Video Handler
        void setupVideoPlayer(float fadein,float fadeout,float enticerFadeIn,string enticerUrl);
    
        //! Load the Video
        void loadVideo(string url,bool enticer);
    
//        void threadedFunction();
//        void start();
//        void stop();
    
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
    
        //! Show Outline
        void setDrawOutline(bool drawOutline);
    
        //! Draw Timeline
        void drawTimeline(int y);
    
        //! How many Seconds Left
        float getTimeLeft();
    
        //! Check if its the Enticer Playing
        bool isEnticerPlaying();
    
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
        ofRPIVideoPlayer videoPlayer;
    //        ofVideoPlayer videoPlayer;
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
        bool _drawOutline;
        string videoName;
        string _enticerUrl;
    
        bool isEnticer;
    
    protected:
        int currentFadeValue;
        float videoLength;
};

#endif /* VideoPlayer_hpp */
