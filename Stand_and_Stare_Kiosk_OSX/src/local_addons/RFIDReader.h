//
//  RFIDReader.h
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef RFIDReader_h
#define RFIDReader_h

#include <stdio.h>
#include "ofMain.h"
#include "MyTimer.h"

class RFIDReader : public ofThread {

public:
    //! Setup the Serial Connection
    void setup(string deviceName,int removeDelay = 2000);
    
    //! Check if the Serial Device is Connected
    bool isConnected();
    
    //! Simulate a new tag being added
    void simulateNewTag(string tagID);
    
    //! Simulate Removal
    void simulateTagRemoval();
    
    //! Start Threading
    void start();
    
    //! Stop Threading
    void stop();
    
    //! Update
    void update();
    
    //! Close
    void closeConnection();
    
    //! Get Debug Information
    string getDebugString();

    //! Get the Current Tag
    string getCurrentTag();
    
    // Serial Object
    ofSerial serial;
    
    // Reconnection Timer
    MyTimer reconnectionTimer;
    void reconnectionTimerStarted(string &str);
    void reconnectionTimerFinished(string &str);
    
    
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
    string _deviceName;
    int connectionAttempts;
    
protected:
    void threadedFunction();
};

#endif /* RFIDReader_h */
