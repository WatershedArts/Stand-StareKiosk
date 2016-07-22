#pragma once

#include "ofMain.h"
//#include "ProjectorControl.h"
#include "DonationReader.h"
#include "LoadSettings.h"
#include "VideoPlayer.h"
#include "Calibration.h"
#include "RFIDReader.h"
#include "Enticer.h"
#include "PostData.h"
#include "ofxDatGui.h"
#include "MyTimer.h"
#include "terminalListener.h"
#include "Assigner.h"
#include "SplashScreen.h"
#include "ArduinoHandler.h"

#define SPLASH_DELAY 2500

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
    MyTimer idleTimer;
    ofImage logo;
    
    deque<VideoData> videoData;
    
    int applicationMode;
    bool canDrawData;
    void drawDebugData();
    bool canPlay;
    bool flipIdleTimerLatch;
    bool calibrateScreen;

    string videoCode;
    string videoDetails;
    string unitName;
    string previousTagId;
    int unitId;
    int timesUsedToday;
    int donationsToday;
    ofEvent<int> donate;
    int videoPlayback;    
    //--------------------------------------------------------------
    // *
    // * Class Listeners
    // *
    //--------------------------------------------------------------
    void setupListeners();
    void removeListeners();
    void videoStarted(string &args);
    void videoFinished(string &args);
    void videoInterupted(string &args);
    void enticerVideoStarted(string &args);
    void enticerVideoFinished(string &args);
    void reconnectRFID(int &args);
    void newTagAdded(string &tag);
    void tagRemoved(int &val);
    void timerStopped(string &timer);
    void gotDonation(int &pin);
    
    //--------------------------------------------------------------
    // *
    // * Screen Warper
    // *
    //--------------------------------------------------------------
    void setupWarper();
    ofxQuadWarp screenWarper;
    ofFbo screenFbo;
    ofImage templateImage;
    bool showTemplate;
    bool showWarper;
    bool useWarper;
    bool stopLoad;
    
    //--------------------------------------------------------------
    // *
    // * GUI
    // *
    //--------------------------------------------------------------
    void setupGUI();
    
    // GUI Object
    ofxDatGui * gui;
    ofRectangle guiWindow;
    
    ofxDatGuiLabel * title;
    ofxDatGuiFRM * appFPS;
    ofxDatGuiDropdown * appMode;
    ofxDatGuiButton * reloadVideoData;
    ofxDatGuiButton * addNewFile;
    ofxDatGuiToggle * showWarperBox;
    ofxDatGuiToggle * showTemplateImage;
    
    ofxDatGuiFolder * calibrationFolder;
    ofxDatGuiFolder * videoFolder;
    
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
//    ProjectorControl projectorController;
    LoadSettings appConfiguration;
    DonationReader donationReader;
    Calibration calibrationScreen;
    VideoPlayer videoHandler;
    EnticerVisuals enticer;
    RFIDReader rfidReader;
    PostData postData;
    TagAssignment tagAssigner;
    ArduinoHandler arduino;
    
    //--------------------------------------------------------------
    // *
    // * Terminal Listener
    // *
    //--------------------------------------------------------------
    void onCharacterReceived(KeyListenerEventData& e);
    TerminalListener consoleListener;

    //--------------------------------------------------------------
    // *
    // * Assigning
    // *
    //--------------------------------------------------------------
    void drawAssigningScreen();
    
    ofTrueTypeFont titleFont;
    SplashScreen splashScreen;
    bool initLoad;
};
