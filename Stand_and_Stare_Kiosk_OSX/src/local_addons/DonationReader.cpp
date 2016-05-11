//
//  DonationReader.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "DonationReader.h"
//--------------------------------------------------------------
void DonationReader::setup(string name)
{
    loadMask();
    donationTimer.setup(5000, "Donation", false);
    waveshed.load("photos/waveshed.jpg");
    
    maskShader.load("shaders/maskShader");
    
    maskFbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.allocate(ofGetWidth(), ofGetHeight());
    blurShader.setup(ofGetWidth(), ofGetHeight());
    
    maskFbo.begin();
    ofClear(0);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    reset();
    
    waveTopPts.clear();
    waveBottomPts.clear();
    
    x = 0;
    
    buildWave = false;
}
//--------------------------------------------------------------
void DonationReader::setSensitivity(float sensitivity)
{
    // This is the Threshold of the Switch
    _sensitivity = sensitivity;
}
//--------------------------------------------------------------
void DonationReader::update()
{
    if(x < ofGetWidth()) {
        int y1Origin = 0;
        int y2Origin = ofGetHeight();
        int y1 = y1Origin+120*ofSignedNoise(ofGetElapsedTimef());
        int y2 = y2Origin+150*ofSignedNoise(ofGetElapsedTimef());
        waveTopPts.push_back(ofVec3f(x,y1,0));
        waveBottomPts.push_back(ofVec3f(x,y2,0));
        x++;
    }
    
    donationTimer.update();
   
    
    maskFbo.begin();
    ofClear(0, 0, 0);
    blurShader.begin(5,5);
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(0,bouncingArea.update());
    ofSetColor(ofColor::white);
    ofBeginShape();
    ofVertex(0, 0);
    for (int i = 0; i < waveTopPts.size(); i++) {
        ofVertex(waveTopPts[i].x, waveTopPts[i].y);
    }
    ofVertex(ofGetWidth(), 0);
    ofVertex(ofGetWidth(), ofGetHeight());
    for (int i = waveBottomPts.size()-1; i > 0 ; i--) {
        ofVertex(waveBottomPts[i].x, waveBottomPts[i].y);
    }
    ofVertex(0, ofGetHeight());
    ofEndShape(false);
    ofPopMatrix();
    
    
    ofPopStyle();
    blurShader.end();
    maskFbo.end();
    
    
}
//--------------------------------------------------------------
void DonationReader::draw(int x, int y)
{
    
    if (!donationTimer.hasTimerFinished()) {
        fbo.begin();
        ofClear(0, 0, 0);
        
        maskShader.begin();
        maskShader.setUniformTexture("maskTex", maskFbo.getTexture(), 1);
        waveshed.draw(0, 0,ofGetWidth(),ofGetHeight());
        maskShader.end();
        fbo.end();
        
        fbo.draw(0,0);
        drawMask();
    }
}
//--------------------------------------------------------------
bool DonationReader::gotDonation()
{
    return donation;
}
//--------------------------------------------------------------
string DonationReader::getStringStream()
{
    stringstream datastream;
    return datastream.str();
}
//--------------------------------------------------------------
void DonationReader::simulateDonation()
{
    donationTimer.start();
    bouncingArea.setParameters(0, linear, ofxTween::easeInOut, -ofGetHeight(), ofGetHeight()*2, 4000, 10);
}
//--------------------------------------------------------------
void DonationReader::close()
{
    
}
//--------------------------------------------------------------
void DonationReader::reset()
{
    donation = false;
}
//--------------------------------------------------------------
void DonationReader::loadMask()
{
    maskPoints.clear();
    if (maskFile.open("configs/mask.json")) {
        int numberOfMasks = maskFile["masks"].size();
        maskPoints.resize(numberOfMasks);
        for (int i = 0; i < numberOfMasks; i++) {
            int maskPts = maskFile["masks"][i]["pts"].size();
            for (int w = 0; w < maskPts; w++) {
                maskPoints[i].push_back(ofVec2f(maskFile["masks"][i]["pts"][w]["x"].asFloat(), maskFile["masks"][i]["pts"][w]["y"].asFloat()));
            }
        }
    }
    else {
        ofLogError() << "Mask File Unavailable";
    }
}
//--------------------------------------------------------------
void DonationReader::drawMask()
{
    ofPushStyle();
    ofSetColor(ofColor::black);
    ofFill();
    for (int i = 0; i < maskPoints.size(); i++) {
        ofBeginShape();
        for (int w = 0; w < maskPoints[i].size(); w++) {
            ofVertex(maskPoints[i][w].x, maskPoints[i][w].y);
        }
        ofEndShape(true);
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void DonationReader::drawMaskOutline()
{
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofNoFill();
    for (int i = 0; i < maskPoints.size(); i++) {
        ofBeginShape();
        for (int w = 0; w < maskPoints[i].size(); w++) {
            ofVertex(maskPoints[i][w].x, maskPoints[i][w].y);
        }
        ofEndShape(true);
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void DonationReader::drawScreens()
{
    ofPushStyle();
    
    for (int i = 0; i < maskPoints.size(); i++) {
        ofSetColor(ofColor::white);
        ofFill();
        ofBeginShape();
        for (int w = 0; w < maskPoints[i].size(); w++) {
            ofVertex(maskPoints[i][w].x, maskPoints[i][w].y);
        }
        ofEndShape(true);
        ofSetColor(ofColor::black);
        ofDrawBitmapString("Screen No: " + ofToString(i), maskPoints[i][0].x, maskPoints[i][0].y);
    }
    ofPopStyle();
    
    
}
