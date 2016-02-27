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
    void Setup(float timerLength,string timerName,bool loop);
    void Update();
    void Draw(int x, int y);
    
    void Start();
    void Stop();
    void Reset();
    void SetNewTimerLength(int timerLength);
    float GetTimeLeft();
    
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
