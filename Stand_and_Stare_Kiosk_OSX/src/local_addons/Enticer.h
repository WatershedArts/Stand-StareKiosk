//
//  Enticer.h
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef Enticer_h
#define Enticer_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxTween.h"
#include "ofxQuadWarp.h"

class EnticerVisuals : public ofThread {
    public:
        //! Setup Video Player
        void setupVideoPlayer(float fadein,float fadeout,float videoHandlerFadeOut);
    
        //! Load the Video
        void loadVideo(string url);

        //! Start Thread
        void start();
    
        //! Stop Thread
        void stop();
    
        //! Threaded Function
        void threadedFunction();
    
        //! Close
        void close();
    
        //! Updates the Video Player
        void updateVideo();

        //! Plays Loaded Video
        void playVideo();
    
        //! Stops the Video
        void stopVideo();
    
        //! Draws
        void drawVideo();
    
        //! Draw Timeline
        void drawTimeline(int y);
    
        //! Draws the internal Video Warper
        void drawCalibrationQuads();
    
        //! Shows the Primary Warping Quads
        void showPrimaryQuad(bool val);

        //! Shows the Secondary Warping Quads
        void showSecondaryQuad(bool val);
    
        //! Get the Time Left to Play
        float getTimeLeft();
    
        //! Has the Video Finished
        bool hasVideoFinished();
    
        //! Is the Video Playing
        bool isVideoPlaying();
    
        //! Get the Debug Data
        string getStringStream();
    
        // Events
        ofEvent<string> enticerStarted;;
        ofEvent<string> enticerInterrupted;
    
    private:
        ofVideoPlayer videoPlayer;
        ofxTween fade;
        ofxTween dropFade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
        ofxQuadWarp warper;
        ofFbo warperFbo;
        string videoName;
        int progress;
        float _fadein;
        float _fadeout;
        float _vHFadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;
        bool _drawPrimaryQuads;
        bool _drawSecondaryQuads;
    protected:
        int currentFadeValue;
        float videoLength;
    
};

#endif /* Enticer_h */
