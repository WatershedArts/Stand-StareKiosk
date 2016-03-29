//
//  MachineSecurity.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 14/12/2015.
//
//

#include "MachineSecurity.hpp"
//--------------------------------------------------------------
void MachineSecurity::setup(int sensor1, int sensor2,string alertSound)
{
    
    _isSensor1Tilted = false;
    _isSensor2Tilted = false;
    
    alarm.load(alertSound);
    
#ifndef PI
    // Instaniate GPIO
    tiltSensor1 = new GPIO(ofToString(sensor1));
    tiltSensor2 = new GPIO(ofToString(sensor2));
    
    // Export the GPIO
    tiltSensor1->export_gpio();
    tiltSensor2->export_gpio();
    
    // Set the direction to in
    tiltSensor1->setdir_gpio("in");
    tiltSensor2->setdir_gpio("in");
    
    // Piezo Element
    piezoElement = new GPIO("8");
    piezoElement->export_gpio();
    piezoElement->setdir_gpio("out");
#endif
}
//--------------------------------------------------------------
void MachineSecurity::update()
{
    if (_isSensor1Tilted || _isSensor2Tilted) {
        cout << "Argh help im being attacked" << endl;
        alarm.play();
        _isSensor1Tilted = false;
        _isSensor2Tilted = false;
    }
#ifndef PI
    bool isSensor1Tilted;
    bool isSensor2Tilted;
    
    tiltSensor1->getval_gpio(isSensor1Tilted);
    tiltSensor2->getval_gpio(isSensor2Tilted);
    
    if (isSensor1Tilted || isSensor2Tilted) {
        cout << "Argh help im being attacked" << endl;
    }
#endif
}
//--------------------------------------------------------------
void MachineSecurity::draw()
{
    
}
//--------------------------------------------------------------
void MachineSecurity::close()
{
    
}
//--------------------------------------------------------------
void MachineSecurity::simulateAlarm()
{
    _isSensor1Tilted = true;
    _isSensor2Tilted = true;
}
