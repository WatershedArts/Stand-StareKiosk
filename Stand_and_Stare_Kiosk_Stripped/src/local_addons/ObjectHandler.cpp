//
//  ObjectHandler.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 16/12/2015.
//
//

#include "ObjectHandler.hpp"
//--------------------------------------------------------------
void ObjectHandler::loadObjects(vector<string> images)
{
    int noObjects = images.size();
    
    for (int i = 0; i < noObjects; i++) {
        imagePlacements.push_back(ofRectangle(ofGetWidth()/2-((noObjects/2)*40)+(i*50), 10, 40, 40));
    }
    
}
//--------------------------------------------------------------
void ObjectHandler::drawObjects()
{
    for (auto objects : imagePlacements) {
        ofSetColor(ofColor::ivory);
        ofDrawRectangle(objects);
    }
}