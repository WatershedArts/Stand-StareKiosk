//
//  SplashScreen.hpp
//  Stand_and_Stare_Kiosk_Stripped
//
//  Created by David Haylock on 29/03/2016.
//
//

#ifndef SplashScreen_hpp
#define SplashScreen_hpp

#include <stdio.h>
#include "ofMain.h"
#include "MyTimer.hpp"

class SplashScreen {
    public:
        //-------------------------------------------
        void load(string image = "/photos/logo.png", string unitName = "", int timerLength = 5000)
        {
            titleFont.load("MuseoSans_500.otf",30);
            logo.load(image);
            splashScreenTimer.setup(timerLength, "Splash", false);
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
            return false;
        }
    
    protected:
        MyTimer splashScreenTimer;
        ofImage logo;
        ofxTween fade;
        ofxEasingLinear linearEasing;
        ofTrueTypeFont titleFont;
        string _unitName;
    
};

#endif /* SplashScreen_hpp */
