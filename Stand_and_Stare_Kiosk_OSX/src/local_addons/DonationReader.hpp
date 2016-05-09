//
//  DonationReader.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

//#define PI

#ifndef DonationReader_hpp
#define DonationReader_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxTween.h"
#include "MyTimer.hpp"
#include "ofxJSON.h"

class DonationReader {
    
public:
    void setup(string name);
    void update();
    void setSensitivity(float sensitivity);
    void draw(int x, int y);
    bool gotDonation();

    // Debug only
    void simulateDonation();
    void close();
    void reset();
    string getStringStream();
    
    void loadMask();
    void drawMask();
    
private:
    float _sensitivity;
    ofxTween bouncingArea;
    ofxEasingBounce bounce;
    ofxEasingLinear linear;
    ofxEasingExpo circ;
    MyTimer donationTimer;
    
    ofImage waveshed;
    ofShader maskShader;
    ofFbo fbo;
    ofFbo maskFbo;
    vector <ofVec3f> waveTopPts;
    vector <ofVec3f> waveBottomPts;
    bool buildWave;
    bool donation;
    int x;
    
    ofxJSONElement maskFile;
    vector<vector<ofVec2f> > maskPoints;
    
protected:

};


#endif /* DonationReader_hpp */
