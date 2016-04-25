//
//  DonationReader.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "DonationReader.hpp"
//--------------------------------------------------------------
void DonationReader::setup(string name)
{
    #ifndef PI
        spi = new SPI(name,SPI_MODE_0,1000000,8);
    #endif
    
    // 5 Sensor Array
    _numberOfSensors = 5;
    donationTimer.setup(5000, "Donation", false);
    reset();
}
//--------------------------------------------------------------
void DonationReader::setSensitivity(float sensitivity)
{
    // This is the Threshold of the Switch
    _sensitivity = sensitivity;
}
//--------------------------------------------------------------
void DonationReader::update()
{
    donationTimer.update();
    for (int ldr = 0; ldr < _numberOfSensors; ldr++) {
        if (ldrValues[ldr] > _sensitivity) {
            donation = true;
        }
    }
}
//--------------------------------------------------------------
void DonationReader::draw(int x, int y)
{
    if (!donationTimer.hasTimerFinished()) {
        ofPushStyle();
        ofSetColor(ofColor::whiteSmoke);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }
}
//--------------------------------------------------------------
bool DonationReader::gotDonation()
{
    return donation;
}
//--------------------------------------------------------------
string DonationReader::getStringStream()
{
    stringstream datastream;
    datastream << "| Reader: " << "he" << endl;
    datastream << "| Video Position: " << "a" << endl;
    datastream << "| Fade Out Time: " << "alpsdf" << endl;
    datastream << "| Video Duration: " << "adg" << endl;
    return datastream.str();
}
//--------------------------------------------------------------
void DonationReader::simulateDonation()
{
    donationTimer.start();
    //    ldrValues[(int)ofRandom(0,5)] = 255;
}
//--------------------------------------------------------------
void DonationReader::close()
{
    
}
//--------------------------------------------------------------
void DonationReader::reset()
{
    for (int ldr = 0; ldr < _numberOfSensors; ldr++) {
        ldrValues[ldr] = 0;
    }
    donation = false;
}