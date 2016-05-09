//
//  SplashScreen.h
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 04/04/2016.
//
//

#ifndef SplashScreen_h
#define SplashScreen_h

#include "ofMain.h"
#include "MyTimer.hpp"

class SplashScreen {
public:
    
    SplashScreen() {}
    ~SplashScreen() {}
    //-------------------------------------------
    void load(string image = "/photos/logo.png", string unitName = "", int timerLength = 1000)
    {
        titleFont.load("fonts/MuseoSans_500.otf",30);
        logo.load(image);
        
        ofAddListener(splashScreenTimer.timerFinished, this, &SplashScreen::timerFinished);
        splashScreenTimer.setup(timerLength, "Splash", false);
        splashScreenTimer.start();
        _unitName = unitName;
    }
    //-------------------------------------------
    void update()
    {
        splashScreenTimer.update();
    }
    //-------------------------------------------
    void draw()
    {
        if (fade.isRunning()) {
            ofSetColor(fade.update());
        }
        else if (!fade.isRunning() && !splashScreenTimer.hasTimerFinished()) {
            ofSetColor(ofColor::white);
        }
        
        int screenCenterX = (ofGetWidth()*0.5);
        int screenCenterY = (ofGetHeight()*0.5);
        int offsetCenterX = (logo.getWidth()*0.5);
        int offsetCenterY = (logo.getHeight()*0.5);
        
        logo.draw((screenCenterX-offsetCenterX),(screenCenterY-offsetCenterY));
        ofRectangle r = titleFont.getStringBoundingBox(_unitName, 0, 0);
        titleFont.drawString(_unitName, (screenCenterX-(r.getWidth()*0.5)), 5+(offsetCenterY+screenCenterY+(r.getHeight())));
    }
    //-------------------------------------------
    bool isDone()
    {
        if (hide && fade.isCompleted())     return true;
        return false;
    }
    //-------------------------------------------
    void timerFinished(string &args)
    {
        fade.setParameters(1, linearEasing, ofxTween::easeInOut, 255, 0, 1000, 0);
        hide = true;
        ofRemoveListener(splashScreenTimer.timerFinished,this, &SplashScreen::timerFinished);
    }
    
protected:
    MyTimer splashScreenTimer;
    ofImage logo;
    ofxTween fade;
    ofxEasingLinear linearEasing;
    ofTrueTypeFont titleFont;
    string _unitName;
    bool hide = false;
    
};


#endif /* SplashScreen_h */
