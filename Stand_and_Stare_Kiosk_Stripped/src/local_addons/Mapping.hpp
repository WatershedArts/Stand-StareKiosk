//
//  Mapping.hpp
//  Stand_and_Stare_Kiosk_Stripped
//
//  Created by David Haylock on 29/03/2016.
//
//

#ifndef Mapping_hpp
#define Mapping_hpp

#include <stdio.h>
#include "ofMain.h"

class Poly {
public:
    
    //--------------------------------------------------------------
    Poly() {
        lineSize = 5;
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
    void setLineSize(int _lineSize) {
        lineSize = _lineSize;
    }
    //--------------------------------------------------------------
    void draw(){
        ofPushStyle();
        ofSetLineWidth(lineSize);
        if (fill) {
            ofFill();
        }
        else {
            ofNoFill();
        }
        
        if (inverts) {
            ofSetColor(255,255,255);
        }
        else {
            ofSetColor(0, 0, 0);
        }
        ofBeginShape();
        for (int i = 0; i < clickPts.size(); i++) {
            ofVertex(clickPts[i].x, clickPts[i].y);
            
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
    int lineSize;
};

class Mapping {
    public:
        void update();
        void draw();
        void deletePoly(int i);
        void mouseDown(int x,int y);
    
    protected:
//        vector<
};

#endif /* Mapping_hpp */
