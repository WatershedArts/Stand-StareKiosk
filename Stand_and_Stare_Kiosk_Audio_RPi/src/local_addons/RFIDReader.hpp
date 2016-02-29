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
#include "ofxGPIO.h"

class RFIDReader : public ofThread {

public:
    void setup(string deviceName,int removeDelay = 2000);
    void update();
    bool isConnected();
    ofSerial serial;

	void start();
	void stop();

	void threadedFunction();

    ofEvent<string> newTag;
    ofEvent<string> tagRemoved;
    
    char incomingByte;
    char tagBuffer[16];
    int tagBufferIndex;
    
    bool tagPresent;
    bool tagPreviouslyPresent;
    bool sendRemove;
    long lastTagChangeTime;
    long removeTimeout;
    
    string getCurrentTag();
    string tagString;
    string getDebugString();

    void simulateNewTag(int which);
    void simulateTagRemoval();

    GPIO * presencePin;
    
protected:

};

#endif /* RFIDReader_hpp */
