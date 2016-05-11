//
//  ProjectorControl.h
//  Stand_StareKiosk
//
//  Created by David Haylock on 16/12/2015.
//
//

#ifndef ProjectorControl_h
#define ProjectorControl_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCasioProjectorControl.h"

class ProjectorControl {
    
public:
    void setupProjector(string projectorSerialName);
    void turnOn();
    void turnOff();
    void close();
    void setMode(int mode);
    bool isConnected();
    string getBulbTime();
    
private:
    ofxCasioProjectorControl projector;
protected:
};

#endif /* ProjectorControl_h */
