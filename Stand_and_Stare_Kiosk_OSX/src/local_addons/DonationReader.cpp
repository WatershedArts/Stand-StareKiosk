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
    donationTimer.setup(3400, "Donation", false);
    waveshed.load("photos/waveshed.jpg");
    

    thankYouNoFilmPlaying.load("videos/thank-you-into.mov");
    thankYouFilmPlaying.load("videos/thank-you-film.mov");
    
    thankYouNoFilmPlaying.setLoopState(OF_LOOP_NONE);
    thankYouFilmPlaying.setLoopState(OF_LOOP_NONE);
    
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
    
    currentFadeAmount = 0;
    x = 0;
    
    donation = true;
    
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
    currentFadeAmount = fade.update();
    thankYouNoFilmPlaying.update();
    thankYouFilmPlaying.update();

    
    float currentVideoTime = (float)(thankYouNoFilmPlaying.getPosition()*thankYouNoFilmPlaying.getDuration());
    if (currentVideoTime >= (thankYouNoFilmPlaying.getDuration()-(1000/1000))) {
        if (!fade.isRunning()) {
            fade.setParameters(1, linear, ofxTween::easeInOut, currentFadeAmount, 0, 500, 1);
        }
    }
    
    float currentVideoTime2 = (float)(thankYouFilmPlaying.getPosition()*thankYouFilmPlaying.getDuration());
    if (currentVideoTime2 >= (thankYouFilmPlaying.getDuration()-(1000/1000))) {
        if (!fade.isRunning()) {
            fade.setParameters(1, linear, ofxTween::easeInOut, currentFadeAmount, 0, 500, 1);
        }
    }
    
    if (!donation && (thankYouNoFilmPlaying.getIsMovieDone() || thankYouFilmPlaying.getIsMovieDone())) {
        donation = true;
    }
}
//--------------------------------------------------------------
bool DonationReader::canDonate()
{
    return donation;
}
//--------------------------------------------------------------
void DonationReader::draw(int x, int y)
{
    if (thankYouNoFilmPlaying.isPlaying()) {
        drawMask();
        ofSetColor(currentFadeAmount,currentFadeAmount);
        thankYouNoFilmPlaying.draw(0, 0);
    }
    
    if (thankYouFilmPlaying.isPlaying()) {
        ofSetColor(currentFadeAmount,currentFadeAmount);
        thankYouFilmPlaying.draw(0, 0);
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
}
//--------------------------------------------------------------
void DonationReader::donatedNoFilmPlaying()
{
    donationTimer.start();
    thankYouNoFilmPlaying.play();
    fade.setParameters(0, linear, ofxTween::easeInOut, currentFadeAmount, 255, 500, 1);
    donation = false;
}
//--------------------------------------------------------------
void DonationReader::donatedFilmPlaying()
{
    donationTimer.start();
    thankYouFilmPlaying.play();
    fade.setParameters(0, linear, ofxTween::easeInOut, currentFadeAmount, 255, 500, 1);
    donation = false;
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
    ofSetColor(0,currentFadeAmount);
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
void DonationReader::drawYellowPlaceholder()
{
    ofPushStyle();
    ofSetHexColor(0Xe4b600);
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
        ofDrawBitmapStringHighlight("Screen No: " + ofToString(i), maskPoints[i][0].x, maskPoints[i][0].y);
    }
    ofPopStyle();
}
//--------------------------------------------------------------
void DonationReader::drawScreens()
{
    ofPushStyle();
    for (int i = 0; i < maskPoints.size(); i++) {
        ofSetHexColor(0XFFFFFF);
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
