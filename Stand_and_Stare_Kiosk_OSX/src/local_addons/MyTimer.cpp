//
//  MyTimer.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "MyTimer.h"

//--------------------------------------------------------------
void MyTimer::setup(float timerLength,string timerName,bool loop)
{
    bTimerReached = true;
    _invalidate = false;
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
        if (!_invalidate) {
            string evt = _timerName + " Finished";
            ofNotifyEvent(timerFinished, evt, this);
        }
        
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
bool MyTimer::hasTimerFinished()
{
    return bTimerReached;
}
//--------------------------------------------------------------
void MyTimer::setNewTimerLength(int timerLength)
{
    _timerLength = timerLength;
}
//--------------------------------------------------------------
void MyTimer::start()
{
    _invalidate = false;
    if (bTimerReached) {
        string evt = _timerName + " Started";
        ofNotifyEvent(timerStarted, evt, this);
        bTimerReached = false;
        startTime = ofGetElapsedTimeMillis();
    }
}
//--------------------------------------------------------------
void MyTimer::stop()
{
    bTimerReached = true;
    _loop = false;
}
//--------------------------------------------------------------
void MyTimer::invalidate()
{
    _invalidate = true;
    string evt = _timerName + " Invalidated";
    ofNotifyEvent(timerInvalidated, evt, this);
}