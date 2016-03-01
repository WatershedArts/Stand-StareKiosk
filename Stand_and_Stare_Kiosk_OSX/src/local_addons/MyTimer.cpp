//
//  MyTimer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "MyTimer.hpp"

//--------------------------------------------------------------
void MyTimer::setup(float timerLength,string timerName,bool loop)
{
    bTimerReached = true;
    _loop = loop;
    _timerLength = timerLength;
    _timerName = timerName;
}
//--------------------------------------------------------------
void MyTimer::update()
{
    float timer = ofGetElapsedTimeMillis() - startTime;
    
    if (!bTimerReached) {
        timeLeft = _timerLength - timer;
    }
    
    if (timer >= _timerLength && !bTimerReached) {
        bTimerReached = true;
        string ev = _timerName + " Finished";
        ofNotifyEvent(timerStarted, ev, this);
        if (_loop) {
            start();
        }
    }
}
//--------------------------------------------------------------
float MyTimer::getTimeLeft()
{
    return timeLeft;
}
//--------------------------------------------------------------
void MyTimer::setNewTimerLength(int timerLength)
{
    _timerLength = timerLength;
}
//--------------------------------------------------------------
void MyTimer::start()
{
    if (bTimerReached) {
        string ev = _timerName + " Started";
        ofNotifyEvent(timerFinished, ev, this);
        
        bTimerReached = false;
        startTime = ofGetElapsedTimeMillis();
    }
}
//--------------------------------------------------------------
void MyTimer::stop( )
{
    bTimerReached = true;
    _loop = false;
}