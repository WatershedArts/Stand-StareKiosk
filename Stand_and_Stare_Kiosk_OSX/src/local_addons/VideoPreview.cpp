//
//  VideoPreview.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 12/01/2016.
//
//

#include "VideoPreview.hpp"
//--------------------------------------------------------------
void VideoPreview::setup(deque <SSVideoData> files)
{
    setButtons = 0;
    _files = files;

    ofAddListener(ofxGifLoadedEvent::events, this, &VideoPreview::gifLoaded);

    loader.startThread();
    
    totalWidth = 0;
    
    for (int i = 0; i < _files.size(); i++) {
        //cout << ofToDataPath(_files[i].gifUrl) << endl;
        loader.loadFromDisk(ofToDataPath(_files[i].gifUrl,true));
    }
    
    saveChanges = SimpleButton(ofGetWidth()/2-25, ofGetHeight()/2+200, 50, 25, "Save", ofColor::yellowGreen);
    discardChanges = SimpleButton(ofGetWidth()/2-25, ofGetHeight()/2+275, 50, 25, "Discard", ofColor::red);
    
    int centreOriginX = (ofGetWidth()/2)-((GIF_WIDTH+GIF_SPACING)*3)/2;
    int centreOriginY = (ofGetHeight()/2)-((GIF_HEIGHT+GIF_SPACING)*2)/2;
    
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            if (y == 0) {
                btnPts.push_back(ofVec2f(centreOriginX +(x*(GIF_WIDTH+GIF_SPACING)), centreOriginY));
            }
            else {
                btnPts.push_back(ofVec2f(centreOriginX +(x*(GIF_WIDTH+GIF_SPACING)), centreOriginY+(GIF_HEIGHT+GIF_SPACING)+10));
            }
        }
    }
}
//--------------------------------------------------------------
void VideoPreview::close()
{
    loader.stopThread();
}
//--------------------------------------------------------------
void VideoPreview::gifLoaded(ofxGifLoadedEvent &e)
{
    if (setButtons < _files.size()) {
        buttons.push_back(SimpleButton(btnPts[setButtons].x, btnPts[setButtons].y, GIF_WIDTH, GIF_HEIGHT, _files[setButtons].RFIDkey, ofColor::seaGreen,e.gif));
        setButtons++;
    }
}
//--------------------------------------------------------------
void VideoPreview::update()
{
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].update();
    }
}
//--------------------------------------------------------------
void VideoPreview::draw()
{
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].draw();
    }
    
    saveChanges.drawStandard();
    discardChanges.drawStandard();
}
//--------------------------------------------------------------
void VideoPreview::mouseOver(int x, int y)
{
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].mouseOver(x, y);
    }
    saveChanges.mouseOver(x, y);
    discardChanges.mouseOver(x, y);
}
//--------------------------------------------------------------
void VideoPreview::mousePressed(int x, int y)
{
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].isActive) {
            buttons[i].isActive = false;
        }
        buttons[i].buttonPressed(x, y, 1);
        saveChanges.buttonPressed(x, y, 1);
        discardChanges.buttonPressed(x, y, 1);
    }
}