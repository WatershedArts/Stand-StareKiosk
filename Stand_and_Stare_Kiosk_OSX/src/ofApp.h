#pragma once

#include "ofMain.h"
#include "ProjectorControl.hpp"
#include "DonationReader.hpp"
#include "ObjectHandler.hpp"
#include "LoadSettings.hpp"
#include "VideoPlayer.hpp"
#include "Calibration.hpp"
#include "RFIDReader.hpp"
#include "Enticer.hpp"
#include "PostData.hpp"
#include "ofxDatGui.h"
#include "MyTimer.hpp"
#include "terminalListener.h"
#include "VideoPreview.hpp"
#include "VideoTimerVisualisation.h"

class ofApp : public ofBaseApp, public KeyListener{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void appSetup();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //--------------------------------------------------------------
    // *
    // * Application Stuff
    // *
    //--------------------------------------------------------------
    ofTrueTypeFont debug;
    int applicationMode;
    bool canDrawData;
    void DrawDebugData();
    void DrawAssigningScreen();
    
    bool canPlay;
    MyTimer idleTimer;
    bool flipIdleTimerLatch;
    bool calibrateScreen;
    string videoCode;
    string videoDetails;
    
    deque<SSVideoData> videoData;
    //--------------------------------------------------------------
    // *
    // * Screen Warper
    // *
    //--------------------------------------------------------------
    void setupWarper();
    ofxQuadWarp screenWarper;
    ofFbo screenFbo;
    bool showWarper;
    bool useWarper;
    
    VideoTimerVisualisation timerVisualisation;
    
    //--------------------------------------------------------------
    // *
    // * GUI
    // *
    //--------------------------------------------------------------
    void setupGUI();
    
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
    
    //--------------------------------------------------------------
    // *
    // * Objects
    // *
    //--------------------------------------------------------------
    ProjectorControl projectorController;
    LoadSettings appConfiguration;
    DonationReader donationReader;
    Calibration calibrationScreen;
    VideoPreview videoPreviewer;
    VideoPlayer videoHandler;
    EnticerVisuals enticer;
    ObjectHandler objects;
    RFIDReader rfidReader;
    PostData postData;
    
    void onCharacterReceived(KeyListenerEventData& e);
    TerminalListener consoleListener;
    
};
