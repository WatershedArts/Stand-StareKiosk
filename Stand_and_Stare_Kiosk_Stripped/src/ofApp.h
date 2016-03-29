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

#define SPLASH_DELAY 5000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
};
