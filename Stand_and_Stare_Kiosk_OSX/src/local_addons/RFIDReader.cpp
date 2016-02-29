//
//  RFIDReader.cpp
//  Stand_StareKiosk
//
//  Modified by David Haylock on 26/11/2015.
//  Created by Dan Williams for Arduino
//

#include "RFIDReader.hpp"
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
    tagString = "";
    
    if (!isConnected()) {
        ofLogError() << "RFID Is Not Connected";
    }
}
//--------------------------------------------------------------
bool RFIDReader::isConnected()
{
    return serial.isInitialized();
}
//--------------------------------------------------------------
void RFIDReader::simulateNewTag(int which)
{
    string ev = "RFIDTag "+ofToString(which);
    ofNotifyEvent(newTag, ev, this);
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
                
                // This is the Removal Script
                // Need to Implement a Arduino based Version
                string state = "0";
                
                if(state == "1") {
                    tagPresent = true;
                }
                else {
                    tagPresent = false;
                }
                
                if(tagPresent != tagPreviouslyPresent) {
                    lastTagChangeTime = ofGetElapsedTimeMillis();;
                    sendRemove = !tagPresent;
                }
                
                if(sendRemove == true && ((ofGetElapsedTimeMillis() - lastTagChangeTime) > removeTimeout)) {
                    string ev = "Tag Removed";
                    ofNotifyEvent(tagRemoved, ev, this);
                    sendRemove = false;
                }
            }
            unlock();
            sleep(1);
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
    datastream << "|----------------------------------" << endl;
    datastream << "| RFID Reader" << endl;
    datastream << "|----------------------------------" << endl;
    datastream << "| Current Tag: " << tagString << endl;
    datastream << "| New Tag " << tagP << endl;
    return datastream.str();
}
