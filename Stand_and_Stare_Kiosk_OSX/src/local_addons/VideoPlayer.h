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

class VideoObject {
    public:
    
    VideoObject() {}
    ~VideoObject() {}
    VideoObject(string url) {
        _fadein = 1000;
        _fadeout = 1000;
        _hasFadedOut = true;
        _hasFadedIn = true;
        _isFadingIn = false;
        _isFadingOut = false;
        videoPlayer.load(url);
        videoPlayer.setLoopState(OF_LOOP_NONE);
        videoLength = videoPlayer.getDuration();
        playVideo();
    }
    
        void load(string url) {
            _fadein = 1000;
            _fadeout = 1000;
            _hasFadedOut = true;
            _hasFadedIn = true;
            _isFadingIn = false;
            _isFadingOut = false;
            videoPlayer.load(url);
//            videoName = url;
//            cout << videoName << endl;
            videoPlayer.setLoopState(OF_LOOP_NONE);
            videoLength = videoPlayer.getDuration();
            playVideo();
        }
        void update() {
            videoPlayer.update();
            currentFadeValue = fade.update();
            videoLength = videoPlayer.getDuration();
            float volumeMap = ofMap(fade.update(), 0, 255, 0.00, 1.00);
            videoPlayer.setVolume(volumeMap);
            
            if (fade.isCompleted() && !_hasFadedOut) {
                videoPlayer.stop();
                if (!videoPlayer.isPlaying()) {
                    cout << "Stopped Video" << endl;
                }
                _hasFadedOut = true;
            }
            
            float currentVideoTime = (float)(videoPlayer.getPosition()*videoPlayer.getDuration());
            if (currentVideoTime >= (videoLength-(_fadeout/1000))) {
                if (_hasFadedOut && isVideoPlaying()) {
                    stopVideo();
                }
            }
        }
        //--------------------------------------------------------------
        void draw()
        {
            ofPushStyle();
            ofSetColor(fade.update(),255);
            if (videoPlayer.isLoaded()) {
                if(videoPlayer.isPlaying()) {
                    videoPlayer.draw(0, 0);
                }
            }
            ofPopStyle();
        }
        //--------------------------------------------------------------
        int getPlayPercentage()
        {
            return videoPlayer.getPosition()*100;
        }
        //--------------------------------------------------------------
        float getTimeLeft()
        {
            return videoPlayer.getPosition();
        }
        //--------------------------------------------------------------
        bool isVideoPlaying()
        {
            return videoPlayer.isPlaying();
        }
        //--------------------------------------------------------------
        void playVideo()
        {
            videoPlayer.play();
            fade.setParameters(1, easinglinear, ofxTween::easeIn, 0, 255, _fadein, 500);
            string ev = "Video Started";
            ofNotifyEvent(videoStarted, ev, this);
            _hasFadedIn = false;
        }
        //--------------------------------------------------------------
        void stopVideo()
        {
            fade.setParameters(1, easinglinear, ofxTween::easeOut, currentFadeValue, 0, _fadeout, 10);
            string ev = "Video Stopped";
            ofNotifyEvent(videoStopped, ev, this);
            _hasFadedOut = false;
        }
        //--------------------------------------------------------------
        void interruptVideo()
        {
            fade.setParameters(1, easinglinear, ofxTween::easeInOut, currentFadeValue, 0, _fadeout, 10);
            string ev = "Video Interrupted";
            ofNotifyEvent(videoInterrupted, ev, this);
            _hasFadedOut = false;
        }
    
        ofEvent<string> videoStarted;
        ofEvent<string> videoStopped;
        ofEvent<string> videoInterrupted;
    
    protected:
        ofVideoPlayer videoPlayer;
        ofxTween fade;
        ofxTween dropFade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
        float _fadein;
        float _fadeout;
        bool _hasFadedIn;
        bool _hasFadedOut;
        
        bool _isFadingIn;
        bool _isFadingOut;
        int currentFadeValue;
        float videoLength;
};

class VideoPlayer {
    
    public:
        //! Setup Video Handler
        void setupVideoPlayer(float fadein,float fadeout,float enticerFadeIn);
    
        //! Load the Video
        void loadVideo(string url);
    
        //! Update video
        void updateVideo();
//    
//        //! Play Video
//        void playVideo();
//    
//        //! Stop Video
//        void stopVideo();
//    
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
    
//        //! How many Seconds Left
//        float getTimeLeft();
//    
//        //! Has the Video Finished
//        bool hasVideoFinished();
//    
//        //! Is the Video Playing
        bool isVideoPlaying();
    
        //! Debug Info
        string getStringStream();
    
//        //! Invalidate The fading system
//        void invalidateFade();
//    
        //! What Percentage through the Video are We?
        int getPlayPercentage();
    
        void fadingResults(string &str);
    
        // Events
        ofEvent<string> videoStarted;
        ofEvent<string> videoStopped;
        ofEvent<string> videoInterrupted;
        vector <VideoObject> videoPlayer;
    private:
//        ofVideoPlayer videoPlayer;
    
        ofxTween fade;
        ofxTween dropFade;
        ofxEasingLinear easinglinear;
        ofxEasingExpo easingexpo;
    
//        ofxQuadWarp warper;
//        ofFbo warperFbo;
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
