//
//  SimpleButton.h
//
//  Created by David Haylock on 15/12/2015.
//
//

#pragma once
#ifndef SimpleButton_h
#define SimpleButton_h

#include "ofMain.h"
#include "ofxThreadedGifFile.h"
//---------------------------------------------------
class SimpleButton : public ofRectangle {
public:
    SimpleButton() {}
    //---------------------------------------------------
    SimpleButton(int x,int y,int w, int h,string name,ofColor c)
    {
        int offset = 4;
        this->set(x-offset,y-offset,w+(offset*2),h+(offset*2));
        standardColor = c;
        buttonName = name;
        isActive = false;
        isOver = false;
    }
    //---------------------------------------------------
    SimpleButton(int x,int y,int w, int h,string name,ofColor c,ofxThreadedGifFile file)
    {
        int offset = 4;
        this->set(x-offset,y-offset,w+(offset*2),h+(offset*2));
        standardColor = c;
        buttonName = name;
        isActive = false;
        isOver = false;
        gif = file;
        counter = 0;
    }
    //---------------------------------------------------
    ~SimpleButton() {}
    //---------------------------------------------------
    void setupButton(int x,int y,int w, int h,string name,ofColor c,ofxThreadedGifFile file)
    {
        int offset = 4;
        this->set(x-offset,y-offset,w+(offset*2),h+(offset*2));
        standardColor = c;
        buttonName = name;
        isActive = false;
        isOver = false;
        counter = 0;
        gif = file;
    }
    //---------------------------------------------------
    void update()
    {
        if (counter > gif.getNumFrames()) {
            counter = 0;
        }
        else {
            if (ofGetFrameNum() % 15 == 0) {
                counter++;
            }
        }
    }
    //---------------------------------------------------
    void draw()
    {
        ofPushStyle();
        if (isOver) {
            ofSetColor(ofColor::orange);
        }
        else if(isActive) {
            ofSetColor(ofColor::aqua);
        }
        else {
            ofSetColor(standardColor);
        }
        
        ofDrawRectangle(this->getStandardized());
        ofSetColor(ofColor::white);
        ofDrawBitmapString(buttonName,this->x,this->y);
        ofPopStyle();
        
        ofPushStyle();
        gif.drawFrame(counter,this->x+4,this->y+4,this->getWidth()-8,this->getHeight()-8);
        ofPopStyle();
        
    }
    //---------------------------------------------------
    void drawStandard()
    {
        ofPushStyle();
        if (isOver) {
            ofSetColor(ofColor::orange);
        }
        else if(isActive) {
            ofSetColor(ofColor::aqua);
        }
        else {
            ofSetColor(standardColor);
        }
        
        ofDrawRectangle(this->getStandardized());
        ofSetColor(ofColor::white);
        ofDrawBitmapString(buttonName,this->x,this->y);
        ofPopStyle();
    }
    //---------------------------------------------------
    void mouseOver(int x,int y)
    {
        if (inside(x,y)) {
            isOver = true;
        }
        else {
            isOver = false;
        }
    }
    //---------------------------------------------------
    void buttonPressed(int x,int y,int button)
    {
        if (inside(x,y)) {
            isActive = !isActive;
        }
    }
    bool isActive = false;
    bool isOver = false;
private:
    
    ofColor standardColor;
    string buttonName;
    ofxThreadedGifFile gif;
    int counter;
};
#endif /* SimpleButton_h */
