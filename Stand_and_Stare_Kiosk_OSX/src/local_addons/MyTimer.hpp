//
//  MyTimer.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef MyTimer_hpp
#define MyTimer_hpp

#include <stdio.h>
#include "ofMain.h"

class MyTimer {
public:
    //! Setup the Timer. Timer Name will be referenced on events
    void setup(float timerLength,string timerName,bool loop);

    //! Update the Timer
    void update();
    
    //! Start the Timer
    void start();
    
    //! Stop the Timer
    void stop();
    
    //! Reset the Timer
    void reset();
    
    //! As it Sounds
    void setNewTimerLength(int timerLength);
    
    //! How long is left on the timer
    float getTimeLeft();
    
    // Events
    ofEvent<string> timerStarted;
    ofEvent<string> timerFinished;
    
private:
    float startTime;
    float endTime;
    float timeLeft;
    bool  bTimerReached;
    bool _loop;
    float _timerLength;
    string _timerName;
};

#endif /* MyTimer_hpp */
