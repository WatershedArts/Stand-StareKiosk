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

class RFIDReader : public ofThread {

public:
    //! Setup the Serial Connection
    void setup(string deviceName,int removeDelay = 2000);
    
    //! Check if the Serial Device is Connected
    bool isConnected();
    
    //! Simulate a new tag being added
    void simulateNewTag(int which);
    
    //! Simulate Removal
    void simulateTagRemoval();
    
    //! Start Threading
    void start();
    
    //! Stop Threading
    void stop();
    
    void update();
    
    //! Get Debug Information
    string getDebugString();

    //! Get the Current Tag
    string getCurrentTag();
    
    // Serial Object
    ofSerial serial;
    
    // Events
    ofEvent<string> newTag;
    ofEvent<string> tagRemoved;

private:
    char incomingByte;
    char tagBuffer[16];
    int tagBufferIndex;
    bool tagPresent;
    bool tagPreviouslyPresent;
    bool sendRemove;
    long lastTagChangeTime;
    long removeTimeout;
    string tagString;
    
protected:
    void threadedFunction();
};

#endif /* RFIDReader_hpp */
