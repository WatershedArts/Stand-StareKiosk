//
//  RFIDReader.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef RFIDReader_hpp
#define RFIDReader_hpp

#include <stdio.h>
#include "ofMain.h"

class RFIDReader {

public:
    void setup(string deviceName,int removeDelay = 2000);
    void update();
    bool isConnected();
    ofSerial serial;

    char incomingByte;
    char tagBuffer[16];
    int tagBufferIndex;
    
    bool tagPresent;
    bool tagPreviouslyPresent;
    bool sendRemove;
    long lastTagChangeTime;
    long removeTimeout;
    string tagString;
    string getDebugString();

protected:

};

#endif /* RFIDReader_hpp */
