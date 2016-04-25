//
//  DonationReader.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

//#define PI

#ifndef DonationReader_hpp
#define DonationReader_hpp

#include <stdio.h>
#include "ofMain.h"

#ifndef PI
#include "ofxGPIO.h"
#endif

#include "MyTimer.hpp"

class DonationReader {
    
public:
    void setup(string name);
    void update();
    void setSensitivity(float sensitivity);
    void draw(int x, int y);
    bool gotDonation();

    // Debug only
    void simulateDonation();
    void close();
    void reset();
    string getStringStream();
    
private:
    float _sensitivity;
    int _numberOfSensors;
    
    
    MyTimer donationTimer;
    
    #ifndef PI
        SPI *spi;
        unsigned char *data;
    #endif
        int ldrValues[5];
        int ldrChannel[5];
        bool donation;
protected:

};


#endif /* DonationReader_hpp */
