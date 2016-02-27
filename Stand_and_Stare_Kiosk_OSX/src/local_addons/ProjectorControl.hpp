//
//  ProjectorControl.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 16/12/2015.
//
//

#ifndef ProjectorControl_hpp
#define ProjectorControl_hpp

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

#endif /* ProjectorControl_hpp */
