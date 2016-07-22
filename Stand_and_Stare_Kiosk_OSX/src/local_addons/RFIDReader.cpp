//
//  RFIDReader.cpp
//  Stand_StareKiosk
//
//  Modified by David Haylock on 26/11/2015.
//  Created by Dan Williams for Arduino
//

#include "RFIDReader.h"
//--------------------------------------------------------------
void RFIDReader::setup(string deviceName,int removeDelay)
{
    // Connect Serial
    serial.setup(deviceName, 9600);
    removeTimeout = removeDelay;
    lastTagChangeTime = 0;
    tagPresent = false;
    tagPreviouslyPresent = false;
    sendRemove = false;
    tagBufferIndex = 0;
    connectionAttempts = 3;
    tagString = "";
    _previousTagId = "";
    _deviceName = deviceName;
    
    ofAddListener(reconnectionTimer.timerStarted, this, &RFIDReader::reconnectionTimerStarted);
    ofAddListener(reconnectionTimer.timerFinished, this, &RFIDReader::reconnectionTimerFinished);
    
    serial.listDevices();
    
    reconnectionTimer.setup(5000, "Reconnection", false);
    
    if (!isConnected()) {
        ofLogError() << "RFID Is Not Connected";
        reconnectionTimer.start();
    }
}
//--------------------------------------------------------------
bool RFIDReader::isConnected()
{
    return serial.isInitialized();
}
//--------------------------------------------------------------
void RFIDReader::simulateNewTag(string tagID)
{
//    string ev = tagID;
    
    tagString = tagID;
//    if (tagString != _previousTagId) {
        ofNotifyEvent(newTag, tagString, this);
//    }
//    else {
//        cout << "Tag has already been placed" << endl;
//    }
    _previousTagId = tagString;
//    ofNotifyEvent(newTag, ev, this);
    
}
//--------------------------------------------------------------
void RFIDReader::simulateTagRemoval()
{
    string ev = "Tag Removed";
    ofNotifyEvent(tagRemoved, ev, this);
}
//--------------------------------------------------------------
void RFIDReader::start()
{
    startThread();
}
//--------------------------------------------------------------
void RFIDReader::stop()
{
    stopThread();
}
//--------------------------------------------------------------
void RFIDReader::update() {
    reconnectionTimer.update();
    // Is the Device Connected
    if (isConnected()) {
        while (serial.available() > 0) {
            
            // Get the Incoming Byte
            incomingByte = serial.readByte();
            
            if (incomingByte > 13) {
                tagBuffer[tagBufferIndex] = incomingByte;
                tagBufferIndex++;
            }
            // Fire the Tag up into the main app
            if (incomingByte == 3) {
                
                tagString = tagBuffer;
//                if (tagString != _previousTagId) {
                    ofNotifyEvent(newTag, tagString, this);
//                }
                
                _previousTagId = tagString;
                
                // Clear the Tag
                for (int i = 0; i < tagBufferIndex; i++) {
                    tagBuffer[i] = 0;
                }
                tagBufferIndex = 0;
            }
        }
        tagPreviouslyPresent = tagPresent;
    }
}
//--------------------------------------------------------------
void RFIDReader::threadedFunction()
{
    // Check if the Thread is running
    while (isThreadRunning()) {
        // Is the resource locked
        if (lock()) {
            // Is the Device Connected
            if (isConnected()) {
                while (serial.available() > 0) {

                    // Get the Incoming Byte
                    incomingByte = serial.readByte();
                    
                    if (incomingByte > 13) {
                        tagBuffer[tagBufferIndex] = incomingByte;
                        tagBufferIndex++;
                    }
                    // Fire the Tag up into the main app
                    if (incomingByte == 3) {
                        tagString = tagBuffer;
                        
                        
                        ofNotifyEvent(newTag, tagString, this);
                        
                        // Clear the Tag
                        for (int i = 0; i < tagBufferIndex; i++) {
                            tagBuffer[i] = 0;
                        }
                        tagBufferIndex = 0;
                    }
                }
                tagPreviouslyPresent = tagPresent;
            }
            unlock();
            sleep(10);
        }
    }
}
//--------------------------------------------------------------
string RFIDReader::getCurrentTag()
{
    if (isConnected()) {
        if (tagString.length() > 0) {
            return tagString;
        }
        else {
            return "No Tag";
        }
    }
    else {
        return "No RFID Reader Attached";
    }
}
//--------------------------------------------------------------
string RFIDReader::getDebugString()
{
    stringstream datastream;
    string connectionStatus = (isConnected())? "Connected" : "Not Connected";
    string tagP = tagPresent ? "True" : "False";
    string isConnectedStr = isConnected() ? "Connected" : "Not Connected";
    datastream << "|----------------------------------" << endl;
    datastream << "| RFID Reader" << endl;
    datastream << "|----------------------------------" << endl;
    datastream << "| Is Connected: " << isConnectedStr << endl;
    datastream << "| Current Tag: " << tagString << endl;
    datastream << "| New Tag " << tagP << endl;
    return datastream.str();
}
//--------------------------------------------------------------
void RFIDReader::closeConnection()
{
    serial.close();
}
//--------------------------------------------------------------
void RFIDReader::reconnectionTimerStarted(string &str)
{
    cout << "Will Attempt to Reconnect in 5 seconds..." << endl;
}
//--------------------------------------------------------------
void RFIDReader::reconnectionTimerFinished(string &str)
{
    cout << "Attempting Reconnection to " << _deviceName << " Attempts Remaining:" << connectionAttempts << " of 3" << endl;
    connectionAttempts--;
    serial.setup(_deviceName, 9600);
    
    if (!isConnected() && connectionAttempts > 0) {
        ofLogError() << "RFID Is Not Connected";
        reconnectionTimer.start();
    }
    
}