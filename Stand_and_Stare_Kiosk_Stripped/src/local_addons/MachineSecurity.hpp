//
//  MachineSecurity.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 14/12/2015.
//
//

#ifndef MachineSecurity_hpp
#define MachineSecurity_hpp

#include <stdio.h>
#include "ofMain.h"

//#define PI
#ifndef PI
#include "ofxGPIO.h"
#endif

class MachineSecurity {
    
public:
    void setup(int sensor1, int sensor2,string alertSound);
    void update();
    void draw();
    void playAlert();
    string GetDebugString();
    void close();
    
    void simulateAlarm();
    ofSoundPlayer alarm;
    
private:
    bool _isSensor1Tilted;
    bool _isSensor2Tilted;
#ifndef PI
    GPIO * tiltSensor1;
    GPIO * tiltSensor2;
    GPIO * piezoElement;
#endif
protected:
    
};

#endif /* MachineSecurity_hpp */
