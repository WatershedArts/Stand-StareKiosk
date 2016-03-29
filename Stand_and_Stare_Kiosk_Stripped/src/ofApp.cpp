#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    splashScreen.load("photos/logo.png", "this");
}
//--------------------------------------------------------------
void ofApp::update()
{
    splashScreen.update();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(ofColor::black);
    
    
    
    splashScreen.draw();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}