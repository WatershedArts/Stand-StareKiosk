//
//  ArduinoHandler.h
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 21/04/2016.
//
//

#ifndef ArduinoHandler_h
#define ArduinoHandler_h

#include <stdio.h>

#include "ofMain.h"
#include "MyTimer.hpp"

class ArduinoHandler {
    public:
        //! Setup the connection to the Arduino
        void setup(string arduinoName,int delayTime);

        void setupPins(int ledPin1,int ledPin2,int TIRPin,int donationPin1,int donationPin2);
    
        //! Initialize the Arduino
        void initializeArduino(const int & version);
    
        //! Update
        void update();
    
        //! Is Connected
        bool isConnected();
    
        //! As it sounds
        void simulateRemoval();
    
        void timerStarted(string &timer);
        void timerFinished(string &timer);
    
        ofEvent<int> rfidTagRemoved;
        ofEvent<int> donationSlot1Event;
        ofEvent<int> donationSlot2Event;
    
        void digitalPinChanged(const int & pinNum);
        void analogPinChanged(const int & pinNum);
    
    private:
        bool _isConnected;
        int _TIRPin;
        int _donationPin1;
        int _donationPin2;
        int _LEDPin1;
        int _LEDPin2;
    
        bool _donationLatch1;
        bool _donationLatch2;
        int _delayTime;
        MyTimer delayTimer;
    
    protected:
        ofArduino arduino;
    
};

#endif /* ArduinoHandler_h */
