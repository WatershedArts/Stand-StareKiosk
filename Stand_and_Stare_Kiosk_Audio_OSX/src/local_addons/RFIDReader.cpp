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
void RFIDReader::simulateNewTag()
{
    string ev = "RFIDTag 1";
    ofNotifyEvent(newTag, ev, this);
}
//--------------------------------------------------------------
void RFIDReader::simulateTagRemoval()
{
    string ev = "Tag Removed";
    ofNotifyEvent(tagRemoved, ev, this);
}
//--------------------------------------------------------------
void RFIDReader::update()
{
    if (isConnected()) {
        while (serial.available() > 0) {

            incomingByte = serial.readByte();

            if (incomingByte > 13) {
                tagBuffer[tagBufferIndex] = incomingByte;
                tagBufferIndex++;
            }

            if (incomingByte == 3) {
                // Fire the Tag up into the main app
                tagString = tagBuffer;
                ofNotifyEvent(newTag, tagString, this);
                
                for (int i = 0; i < tagBufferIndex; i++) {
                    tagBuffer[i] = 0;
                }
                tagBufferIndex = 0;
            }
        }
    
        tagPreviouslyPresent = tagPresent;

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
            string ev = "Removed";
            ofNotifyEvent(tagRemoved, ev, this);
            sendRemove = false;
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
