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
        ofMessage msg(_timerName + " Finished");
        ofSendMessage(msg);
        if (_loop) {
            start();
        }
    }
}
//--------------------------------------------------------------
void MyTimer::draw(int x, int y)
{
    
//    stringstream time;
//    float timeString;
//    //    double fractpart;
//    if (bTimerReached) {
//        timeString = (_timerLength/1000)/60;
//    }
//    else {
//        timeString = (timeLeft/1000)/60;
//    }
//    
//    int mins = ( timeString < 1 ? 0 : 1 );
//    double seconds;
//    double mseconds;
//    float secs = modf(timeString, &seconds);
//    float misecs = modf((secs*60), &mseconds);
//    time << mins << ":" << (int)(secs*60) << endl;
//    ofSetColor(255, 255, 255);
//    
//    ofRectangle r = font.getStringBoundingBox(time.str(), x,y);
//    
//    font.drawString(time.str(), r.getCenter().x,r.getCenter().y);
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
        ofMessage msg(_timerName + " Started");
        ofSendMessage(msg);
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