//
//  Calibration.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 30/11/2015.
//
//

#ifndef Calibration_hpp
#define Calibration_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"

class Poly {
public:
    
    //--------------------------------------------------------------
    Poly() {
        _lineSize = 2;
    }
    //--------------------------------------------------------------
    void addID(int pid) {
        _id = pid;
    }
    //--------------------------------------------------------------
    void addPoint(ofVec3f pos) {
        clickPts.push_back(pos);
    }
    //--------------------------------------------------------------
    void clear() {
        clickPts.clear();
    }
    //--------------------------------------------------------------
    void setInverse(bool invert) {
        inverts = invert;
    }
    //--------------------------------------------------------------
    void setFill(bool _fill) {
        fill = _fill;
    }
    //--------------------------------------------------------------
    void setLineSize(int lineSize) {
        lineSize = _lineSize;
    }
    //--------------------------------------------------------------
    void showCoordinates(bool show) {
        _showCoords = show;
    }
    //--------------------------------------------------------------
    void draw(){
        ofPushStyle();
        ofSetLineWidth(_lineSize);
        if (fill) {
            ofFill();
        }
        else {
            ofNoFill();
        }
       
        ofBeginShape();
        for (int i = 0; i < clickPts.size(); i++) {
            ofSetColor(ofColor::white);
            ofVertex(clickPts[i].x, clickPts[i].y);
            if (_showCoords) {
                ofSetColor(ofColor::blueSteel);
                ofDrawCircle(clickPts[i].x, clickPts[i].y,5);
                stringstream crd;
                crd << "X" << clickPts[i].x << endl;
                crd << "Y" << clickPts[i].y << endl;
                ofDrawBitmapString(crd.str(), clickPts[i].x, clickPts[i].y);
            }
            
        }
        ofEndShape(true);
        ofPopStyle();
        
    }
    //--------------------------------------------------------------
    vector <ofVec3f> getPts() {
        return clickPts;
    }
    //--------------------------------------------------------------
    int getID() {
        return _id;
    }
    
protected:
    vector <ofVec3f> clickPts;
    bool inverts;
    bool fill;
    int _id;
    int _lineSize;
    bool _showCoords;
};

//--------------------------------------------------------------
class Calibration {
public:
    void setup(vector<ofPoint> pts);
    void update();
    void draw();
    void mousePressed(int x,int y,int button);
    void mouseReleased(int x,int y,int button);
    void clearPoints();
    void showCoordinates(bool showCoordinates);
    void save();
    void load();
    vector<ofVec3f> getCoordinates();
    
private:
    vector<Poly> lines;
    Poly p;
protected:
    
};



#endif /* Calibration_hpp */
