#pragma once

#include "ofMain.h"
#include "ProjectorControl.hpp"
#include "terminalListener.h"
#include "DonationReader.hpp"
#include "ObjectHandler.hpp"
#include "LoadSettings.hpp"
#include "VideoPlayer.hpp"
#include "RFIDReader.hpp"
#include "PostData.hpp"
#include "ofxDatGui.h"
#include "MyTimer.hpp"
#include "Assigner.hpp"
#include "SplashScreen.hpp"

#define SPLASH_DELAY 5000

enum APPLICATION_MODE {
    
    ASSIGNMENT_MODE = 0,
    PLAYBACK_MODE,
    MAPPING_MODE,
};

class ofApp : public ofBaseApp, public KeyListener {

	public:
		void setup();
		void update();
		void draw();

        void exit();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
    
        int applicationMode;
        ofTrueTypeFont debugFont;
    
        SplashScreen splashScreen;
        bool initLoad;
    
        VideoPlayer videoHandler;
        string cuedVideoUrl;
        string videoCode;
        int timesUsedToday;
    
        PostData postData;
        LoadSettings appSettings;
        RFIDReader rfidReader;
        TagAssigner assigner;
    
        MyTimer delayTimer;
        bool playEnticer;
    
        //--------------------------------------------------------------
        // *
        // * Class Listeners
        // *
        //--------------------------------------------------------------
        void setupListeners();
        void removeListeners();

        // RFID Listeners
        void tagAdded(string &tag);
        void tagRemoved(string &tag);
    
        // Video Listeners
        void videoStarted(string &args);
        void videoFinished(string &args);
        void videoInterupted(string &args);
    
        void delayTimerStarted(string &args);
        void delayTimerFinished(string &args);
    
        //--------------------------------------------------------------
        // *
        // * Terminal Listener
        // *
        //--------------------------------------------------------------
        void onCharacterReceived(KeyListenerEventData& e);
        TerminalListener consoleListener;
    
        //--------------------------------------------------------------
        // *
        // * GUI
        // *
        //--------------------------------------------------------------
        void setupGUI();
        bool drawGui;
        // GUI Object
        ofxDatGui * gui;
        ofRectangle guiWindow;
        
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        void on2dPadEvent(ofxDatGui2dPadEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
        void onMatrixEvent(ofxDatGuiMatrixEvent e);
};
