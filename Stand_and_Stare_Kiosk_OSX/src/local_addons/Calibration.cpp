//
//  Calibration.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 30/11/2015.
//
//

#include "Calibration.h"
//--------------------------------------------------------------
void Calibration::setup(vector<ofPoint> pts)
{
    for (int i = 0; i < pts.size(); i++) {
        p.addPoint(pts[i]);
    }
    
    lines.push_back(p);
    p.clear();
}
//--------------------------------------------------------------
void Calibration::update()
{
    
}
//--------------------------------------------------------------
void Calibration::clearPoints()
{
    lines.clear();
    p.clear();
}
//--------------------------------------------------------------
void Calibration::showCoordinates(bool showCoordinates)
{
    p.showCoordinates(showCoordinates);
}
//--------------------------------------------------------------
void Calibration::draw()
{
    if (lines.size() > 0) {
        for(int i = 0; i < lines.size(); i++) {
            lines[i].setFill(false);
            lines[i].setInverse(true);
            lines[i].setLineSize(1);
            ofSetColor(ofColor::white);
            lines[i].draw();
        }
    }
    
    p.setFill(false);
    p.setInverse(true);
    p.setLineSize(1);
    ofSetColor(ofColor::white);
    p.draw();
    
    if (true) {
        ofSetColor(255);
    }
    else {
        ofSetColor(0);
    }
    
    ofSetColor(ofColor::white);
    ofDrawLine(ofGetMouseX()-15, ofGetMouseY(), ofGetMouseX()+15, ofGetMouseY());
    ofDrawLine(ofGetMouseX(), ofGetMouseY()-15, ofGetMouseX(), ofGetMouseY()+15);
}
//--------------------------------------------------------------
vector<ofVec3f> Calibration::getCoordinates()
{
    if (!lines.empty()) {
        return lines[0].getPts();
    }
    else {
        return p.getPts();
    }
}
//--------------------------------------------------------------
void Calibration::mousePressed(int x, int y, int button)
{
    if (button == OF_MOUSE_BUTTON_LEFT) {
        p.addPoint(ofVec3f(x,y,0));
    }
}

void Calibration::mouseReleased(int x,int y,int button)
{
    
}
//--------------------------------------------------------------
void Calibration::save()
{
    ofxJSONElement file;
    file.open("config.json");
    vector<ofPoint> maskPoints;
    int numberOfMaskPoints = file["Stand&Stare"]["MaskPoints"].size();
    file["Stand&Stare"]["MaskPoints"].clear();
    for (int pts = 0; pts < p.getPts().size(); pts++) {
        file["Stand&Stare"]["MaskPoints"][pts]["x"] = p.getPts()[pts].x;
        file["Stand&Stare"]["MaskPoints"][pts]["y"] = p.getPts()[pts].y;
    }
    file.toStyledString();
    file.save("config.json",true);
    
    for (int z = 0; z < p.getPts().size(); z++) {
        cout << p.getPts()[z] << endl;
    }
}