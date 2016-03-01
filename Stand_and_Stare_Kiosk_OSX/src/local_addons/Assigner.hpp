//
//  Assigner.hpp
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 01/03/2016.
//
//

#ifndef Assigner_hpp
#define Assigner_hpp

#include <stdio.h>
#include "ofMain.h"
#include "LoadSettings.hpp"
#include "ofxModal.h"


class TagInformation : public ofRectangle {
    public:
        // Constructor
        TagInformation() {}
        // Deconstructor
        ~TagInformation() {}
        // Constructor
        TagInformation(ofPoint pos,int id, string videoName, string videoDescription, string tagKey, string tagIcon, string videoLength) {
            
            this->set(pos.x, pos.y, 300, 100);
        
            _id = id;
            _videoName = videoName;
            _videoDescription = videoDescription;
            _tagKey = tagKey;
            _tagIcon = tagIcon;
            _videoLength = videoLength;
        }
    
        bool isActive() {
            return isSelected;
        }
    
        void setDeactivated() {
            isSelected = false;
        }
    
        void setNewTag(string newTag) {
            _tagKey = newTag;
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
            stringstream ss;
            ss << _id << endl;
            ss << _videoName << endl;
            ss << _tagKey << endl;
            ss << _tagIcon << endl;
            ss << _videoLength << endl;
            
            ofDrawRectangle(this->getStandardized());
            ofSetColor(ofColor::white);
            ofDrawBitmapString(ss.str(), this->getX()+10,this->getY()+15);
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
        string _videoName;
        string _videoDescription;
        string _tagKey;
        string _tagIcon;
        string _videoLength;
        string _s;
        bool isHover = false;
        bool isSelected = false;
    
};

class TagAssignment {
    public:
        //! Setup
        void setup(deque <SSVideoData> data);
        
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
        
        ofxModalConfirm confirmWindow;
        void onModalEvent(ofxModalEvent e);
    
    private:
        string currentTag;
        map<int,TagInformation> information;
};

#endif /* Assigner_hpp */
