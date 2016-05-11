//
//  ProjectorControl.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 16/12/2015.
//
//

#include "ProjectorControl.h"
//--------------------------------------------------------------
void ProjectorControl::setupProjector(string projectorSerialName)
{
    projector.openProjectorConnection(projectorSerialName);
    if (!isConnected()) {
        ofLogError() << "No Projector Connected";
    }
}
//--------------------------------------------------------------
void ProjectorControl::turnOn()
{
    if (projector.isConnected()) {
        projector.turnProjectorOn();
    }
}
//--------------------------------------------------------------
void ProjectorControl::turnOff()
{
    if (projector.isConnected()) {
        projector.turnProjectorOff();
    }
}
//--------------------------------------------------------------
void ProjectorControl::setMode(int mode)
{
    if (projector.isConnected()) {
        projector.setProjectionMode(mode);
    }
}
//--------------------------------------------------------------
string ProjectorControl::getBulbTime()
{
    return ofToString(projector.getLampHours());
}
//--------------------------------------------------------------
bool ProjectorControl::isConnected()
{
    return projector.isConnected();
}
//--------------------------------------------------------------
void ProjectorControl::close()
{
    projector.closeConnection();
}