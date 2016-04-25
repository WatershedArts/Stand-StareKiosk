//
//  ArduinoHandler.cpp
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 21/04/2016.
//
//

#include "ArduinoHandler.h"


//--------------------------------------------------------------
void ArduinoHandler::setup(string arduinoName,int delayTime)
{
    arduino.connect(arduinoName, 57600);
    ofAddListener(arduino.EInitialized, this, &ArduinoHandler::initializeArduino);
    
    _isConnected = false;
    
    delayTimer.setup(delayTime, "RFID Delay", false);
    ofAddListener(delayTimer.timerStarted,this,&ArduinoHandler::timerStarted);
    ofAddListener(delayTimer.timerFinished,this,&ArduinoHandler::timerFinished);
}
//--------------------------------------------------------------
void ArduinoHandler::timerStarted(string &timer)
{
    cout << "Timer Started" << endl;
}
//--------------------------------------------------------------
void ArduinoHandler::timerFinished(string &timer)
{
    cout << "Timer Finished" << endl;
    int v = 8;
    ofNotifyEvent(rfidTagRemoved, v, this);
}
//--------------------------------------------------------------
void ArduinoHandler::update()
{
    arduino.update();
    delayTimer.update();
}
//--------------------------------------------------------------
bool ArduinoHandler::isConnected()
{
    return _isConnected;
}
//--------------------------------------------------------------
void ArduinoHandler::simulateRemoval()
{
    int v = 8;
    ofNotifyEvent(rfidTagRemoved, v, this);
}
//--------------------------------------------------------------
void ArduinoHandler::initializeArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(arduino.EInitialized, this, &ArduinoHandler::initializeArduino);
    
    // it is now safe to send commands to the Arduino
    _isConnected = true;
    _donationLatch1 = false;
    _donationLatch2 = false;
    
    // print firmware name and version to the console
    ofLogNotice() << arduino.getFirmwareName();
    ofLogNotice() << "firmata v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();
    
    // set pins D2 and A5 to digital input
    arduino.sendDigitalPinMode(2, ARD_OUTPUT);
    arduino.sendDigitalPinMode(8, ARD_INPUT);

    // set pin A0 to analog input
    arduino.sendAnalogPinReporting(0, ARD_ANALOG);

    // Listen for changes on the digital and analog pins
    ofAddListener(arduino.EDigitalPinChanged, this, &ArduinoHandler::digitalPinChanged);
    ofAddListener(arduino.EAnalogPinChanged, this, &ArduinoHandler::analogPinChanged);
    
    arduino.sendDigital(2, ARD_HIGH);
}
//--------------------------------------------------------------
void ArduinoHandler::digitalPinChanged(const int & pinNum)
{
    if (pinNum == 8) {
        if (arduino.getDigital(8) == ARD_LOW) {
            delayTimer.start();
        }
    }
}
//--------------------------------------------------------------
void ArduinoHandler::analogPinChanged(const int & pinNum)
{
    if (pinNum == 0) {
        int v = pinNum;
        if (!_donationLatch1 && arduino.getAnalog(pinNum) < 940) {
            ofNotifyEvent(donationSlot1Event, v, this);
            cout << arduino.getAnalog(pinNum) << endl;
            _donationLatch1 = true;
        }
        else if (_donationLatch1 && arduino.getAnalog(pinNum) > 940) {
            _donationLatch1 = false;
        }
    }
    else if (pinNum == 1) {
        int v = pinNum;
        if (!_donationLatch2 && arduino.getAnalog(pinNum) < 940) {
            ofNotifyEvent(donationSlot2Event, v, this);
            cout << arduino.getAnalog(pinNum) << endl;
            _donationLatch2 = true;
        }
        else if (_donationLatch2 && arduino.getAnalog(pinNum) > 940) {
            _donationLatch2 = false;
        }
    }
}