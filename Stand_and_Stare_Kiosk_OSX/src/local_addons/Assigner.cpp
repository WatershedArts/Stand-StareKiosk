//
//  Assigner.cpp
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 01/03/2016.
//
//

#include "Assigner.hpp"
//--------------------------------------------------------------
void TagAssignment::setup(deque <VideoData> data)
{
    confirmWindow.addListener(this, &TagAssignment::onModalEvent);
    int centerX = ofGetWidth()*0.5;
    int offsetX = 300/2;
    int newCenterX = centerX-offsetX;
    
    for (int i = 0; i < data.size(); i++) {
        information.insert(std::pair<int, TagInformation>(i, TagInformation(ofPoint(newCenterX,100+(i*120)),i,data[i])));
    }
}
//--------------------------------------------------------------
void TagAssignment::assignNewTag(string newTag)
{
    currentTag = newTag;
    for (int i = 0; i < information.size(); i++) {
        if (information.at(i).isActive()) {
            confirmWindow.setMessageAlignment(ofxParagraph::ALIGN_LEFT);
            confirmWindow.setMessage("Video will now have the RFID Key of " + newTag);
            confirmWindow.show();
        }
    }
}
//--------------------------------------------------------------
void TagAssignment::draw()
{
    for (int i = 0; i < information.size(); i++) {
        information.at(i).draw();
    }
}
//--------------------------------------------------------------
void TagAssignment::mouseOver(int x, int y)
{
    for (int i = 0; i < information.size(); i++) {
        information.at(i).isMouseOver(x, y);
    }
}
//--------------------------------------------------------------
void TagAssignment::mousePressed(int x, int y, int button)
{
    for (int i = 0; i < information.size(); i++) {
        information.at(i).isMousePressed(x, y,button);
    }
}
//--------------------------------------------------------------
void TagAssignment::onModalEvent(ofxModalEvent e)
{
    if (e.type == ofxModalEvent::CANCEL){

    }
    else if (e.type == ofxModalEvent::CONFIRM){
        for (int i = 0; i < information.size(); i++) {
            if (information.at(i).isActive()) {
                cout << currentTag << endl;
                information.at(i).setNewTag(currentTag);
                save(i);
            }
            information.at(i).setDeactivated();
        }
    }
}
//--------------------------------------------------------------
void TagAssignment::save(int which)
{
    ofxJSONElement file;
    file.open(ofToDataPath("configs/videoConfig.json"));
    file["VideoData"]["videoslist"][which]["rfidkey"] = currentTag;
    file.toStyledString();
    file.save("configs/videoConfig.json",true);    
}
