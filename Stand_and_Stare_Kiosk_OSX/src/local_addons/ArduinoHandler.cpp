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
    ofSerial s;
    s.listDevices();
    vector <ofSerialDeviceInfo> deviceList = s.getDeviceList();
    
    for (int i = 0; i < deviceList.size(); i++) {
        if (ofIsStringInString(deviceList[i].getDeviceName(), "tty.usbmodem")) {
            cout << "Connecting" << endl;
            _arduinoName = "/dev/"+deviceList[i].getDeviceName();
            arduino.connect(_arduinoName,57600);
            ofAddListener(arduino.EInitialized, this, &ArduinoHandler::initializeArduino);
        }
    }
    
    delayTimer.setup(delayTime, "RFID Delay", false);
    ofAddListener(delayTimer.timerStarted,this,&ArduinoHandler::timerStarted);
    ofAddListener(delayTimer.timerFinished,this,&ArduinoHandler::timerFinished);
}
//--------------------------------------------------------------
void ArduinoHandler::setupPins(int ledPin1, int ledPin2, int TIRPin, int donationPin1, int donationPin2)
{
    _LEDPin1 = ledPin1;
    _LEDPin2 = ledPin2;
    _TIRPin = TIRPin;
    _donationPin1 = donationPin1;
    _donationPin2 = donationPin2;
}
//--------------------------------------------------------------
void ArduinoHandler::timerStarted(string &timer) {       }
//--------------------------------------------------------------
void ArduinoHandler::timerFinished(string &timer)
{
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
    ofLogNotice() << "Firmata v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();
    
    // set pins D2 and A5 to digital input
    arduino.sendDigitalPinMode(_LEDPin1, ARD_OUTPUT);
    arduino.sendDigitalPinMode(_LEDPin2, ARD_OUTPUT);
    arduino.sendDigitalPinMode(_TIRPin, ARD_INPUT);

    // set pin A0 to analog input
    arduino.sendAnalogPinReporting(_donationPin1, ARD_ANALOG);
    arduino.sendAnalogPinReporting(_donationPin2, ARD_ANALOG);

    // Listen for changes on the digital and analog pins
    ofAddListener(arduino.EDigitalPinChanged, this, &ArduinoHandler::digitalPinChanged);
    ofAddListener(arduino.EAnalogPinChanged, this, &ArduinoHandler::analogPinChanged);
    
    arduino.sendDigital(_LEDPin1, ARD_HIGH);
    arduino.sendDigital(_LEDPin2, ARD_HIGH);
}
//--------------------------------------------------------------
string ArduinoHandler::getDebugString()
{
    stringstream datastream;
    string isConnectedStr = isConnected() ? "Connected" : "Not Connected";
    datastream << "|----------------------------------" << endl;
    datastream << "| Arduino" << endl;
    datastream << "|----------------------------------" << endl;
    datastream << "| Arduino Name: " << _arduinoName << endl;
    datastream << "| Is Connected: " << isConnectedStr << endl;
    datastream << "| Analog 0: " << arduino.getAnalog(_donationPin1) << endl;
    datastream << "| Analog 1: " << arduino.getAnalog(_donationPin2) << endl;
    datastream << "| RFID TIR: " << arduino.getDigital(_TIRPin) << endl;

    return datastream.str();
}
//--------------------------------------------------------------
void ArduinoHandler::digitalPinChanged(const int & pinNum)
{
    if (pinNum == _TIRPin) {
        if (arduino.getDigital(_TIRPin) == ARD_LOW) {
            delayTimer.start();
        }
    }
}
//--------------------------------------------------------------
void ArduinoHandler::analogPinChanged(const int & pinNum)
{
    if (pinNum == _donationPin1) {
        int v = pinNum;
        if (!_donationLatch1 && arduino.getAnalog(pinNum) < 880) {
            ofNotifyEvent(donationSlot1Event, v, this);
            _donationLatch1 = true;
        }
        else if (_donationLatch1 && arduino.getAnalog(pinNum) > 880) {
            _donationLatch1 = false;
        }
    }
    else if (pinNum == _donationPin2) {
        int v = pinNum;
        if (!_donationLatch2 && arduino.getAnalog(pinNum) < 880) {
            ofNotifyEvent(donationSlot2Event, v, this);
            _donationLatch2 = true;
        }
        else if (_donationLatch2 && arduino.getAnalog(pinNum) > 880) {
            _donationLatch2 = false;
        }
    }
}