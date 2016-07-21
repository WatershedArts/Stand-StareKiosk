//
//  Assigner.h
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 01/03/2016.
//
//

#ifndef Assigner_h
#define Assigner_h

#include <stdio.h>
#include "ofMain.h"
#include "LoadSettings.h"
#include "ofxModal.h"
#include "ConfirmWindow.h"


class TagInformation : public ofRectangle {
    public:
        // Constructor
        TagInformation() {}
        // Deconstructor
        ~TagInformation() {}
        // Constructor
        TagInformation(ofPoint pos,int id,VideoData vd) {
            this->set(pos.x, pos.y, 300, 100);
            _id = id;
            data = vd;
        }
    
        bool isActive() {
            return isSelected;
        }
    
        void setDeactivated() {
            isSelected = false;
        }
    
        void setNewTag(string newTag) {
            data.RFIDkey = newTag;
        }
    
        void draw() {
            ofPushStyle();
            ofNoFill();
            
            if (isHover) {
                ofSetColor(ofColor::green);
            }
            else if(isSelected) {
                ofSetColor(ofColor::red);
            }
            else {
                ofSetColor(ofColor::white);
            }
            ofSetLineWidth(3);
            ofDrawRectRounded(this->getStandardized(),5);
            ofSetColor(ofColor::white);
            ofDrawBitmapString("Video " + ofToString(_id), this->getX()+10,this->getY()+15);
            ofDrawBitmapString(data.getData(), this->getX()+10,this->getY()+30);
            ofPopStyle();
        }
    
        void isMouseOver(int x, int y) {
            if (inside(x, y)) {
                isHover = true;
            }
            else {
                isHover = false;
            }
        }
    
        void isMousePressed(int x, int y ,int button) {
            if (inside(x, y)) {
                isSelected = !isSelected;
            }
        }
    
    private:
        int _id;
        VideoData data;
        bool isHover = false;
        bool isSelected = false;
    
};

class TagAssignment {
    public:
        //! Setup
        void setup(deque <VideoData> data);
        
        //! Update
        void update();

        //! Draw
        void draw();
        
        //! Assign New Tag
        void assignNewTag(string newTag);
        
        //! Mouse Pressed
        void mousePressed(int x,int y,int button);
        
        //! Mouse Over
        void mouseOver(int x,int y);
    
        //! Save the New RFID Tag
        void save(int which);
    
        //! Add new File
        void addNewFile();
    
        //! Reload Video Data
        void reloadData(deque <VideoData> data);
    
        ConfirmWindow confirmWindow;
    //        ofxModalConfirm confirmWindow;
        void onModalEvent(ofxModalEvent e);
    
    private:
        string currentTag;
        map<int,TagInformation> information;
};

#endif /* Assigner_h */
