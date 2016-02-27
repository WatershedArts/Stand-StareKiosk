//
//  VideoTimerVisualisation.h
//  Stand_StareKiosk
//
//  Created by David Haylock on 13/01/2016.
//
//

#ifndef VideoTimerVisualisation_h
#define VideoTimerVisualisation_h

#include "ofMain.h"
#include "ofxTween.h"

class VideoTimerVisualisation {
    public:
    //--------------------------------------------------------------
    VideoTimerVisualisation() {}
    //--------------------------------------------------------------
    VideoTimerVisualisation(int duration)
    {
        _duration = duration;
    }
    //--------------------------------------------------------------
    void setup(int duration)
    {
        _duration = duration;
    }
    //--------------------------------------------------------------
    void setCurrentDuration(int currentTime)
    {
        _duration = currentTime;
    }
    //--------------------------------------------------------------
    void draw(int x, int y)
    {
        ofPushStyle();
        ofSetColor(ofColor::green);
        ofDrawRectRounded(0,0,25,200,8);
        ofSetColor(0);
        ofFill();
        

        ofDrawRectRounded(0,0,25,_duration,8);
        ofSetColor(255);
        ofNoFill();
        ofDrawRectRounded(0,0,25,200,8);
        ofPopStyle();
    }
    
    protected:
        int _duration;
};
#endif /* VideoTimerVisualisation_h */
