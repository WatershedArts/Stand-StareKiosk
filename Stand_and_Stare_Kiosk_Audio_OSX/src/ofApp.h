#pragma once

#include "ofMain.h"
#include "Enticer.hpp"
#include "MyTimer.hpp"
#include "PostData.hpp"
#include "RFIDReader.hpp"
#include "AudioPlayer.hpp"
#include "LoadSettings.hpp"
#include "terminalListener.h"

class ofApp : public ofBaseApp,public KeyListener{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void gotMessage(ofMessage msg);

        void appSetup();
        void onCharacterReceived(KeyListenerEventData& e);
    
        // Objects
        TerminalListener consoleListener;
        AudioPlayer audioHandler;
        EnticerVisuals enticer;
        LoadSettings appConfig;
        RFIDReader rfidReader;
        PostData postData;
    
        // Listeners
        void trackStarted(string &args);
        void trackFinished(string &args);
        void trackInterupted(string &args);
        void newTagAdded(string &tag);
        void tagRemoved(string &tag);
    
        // Custom
        void drawLabels();
    
        string whatsHappening;
        ofTrueTypeFont titleFont;
        ofTrueTypeFont defaultFont;
    
        deque <SSAudioData> audioData;
        bool hideDebug;
};
