//
//  LoadSettings.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "LoadSettings.h"
//--------------------------------------------------------------
void LoadSettings::loadConfig(string loadFile)
{
 
    if (!configFile.open(loadFile)) {
        // Oh No!!!
        ofLog(OF_LOG_FATAL_ERROR,"Could not find the Configuration File");
        return;
    }
    else {
        // We're good lets get that config!!!
        ofLog(OF_LOG_NOTICE,"Found the Configuration File");
        
        // General Settings
        c.unitName = configFile["Stand&Stare"]["General"]["unitname"].asString();
        c.unitId = configFile["Stand&Stare"]["General"]["unitid"].asInt();
        c.fadeInTime = configFile["Stand&Stare"]["General"]["fadeintime"].asFloat();
        c.fadeOutTime = configFile["Stand&Stare"]["General"]["fadeouttime"].asFloat();
        c.ldrSensitivity = configFile["Stand&Stare"]["General"]["ldrsensitivity"].asFloat();
        c.setFullscreen = configFile["Stand&Stare"]["General"]["fullscreen"].asBool();
        c.useWarper = configFile["Stand&Stare"]["General"]["warperactive"].asBool();
        
        // Post Settings
        c.postHostURL = configFile["Stand&Stare"]["PostData"]["posthost"].asString();
        c.postExtURL = configFile["Stand&Stare"]["PostData"]["postext"].asString();
        c.secretKey = configFile["Stand&Stare"]["PostData"]["secretkey"].asString();
        
        // RFID Settings
        c.RFIDSerialName = configFile["Stand&Stare"]["RFID"]["name"].asString();
        c.rfidBaud = configFile["Stand&Stare"]["RFID"]["baud"].asInt();
        c.rfidDelay = configFile["Stand&Stare"]["RFID"]["delay"].asInt();
        
        // Enticer Settings
        c.enticerVideoUrl = configFile["Stand&Stare"]["Enticer"]["videourl"].asString();
        c.enticerFadeOut = configFile["Stand&Stare"]["Enticer"]["fadeout"].asFloat();
        c.enticerFadeIn = configFile["Stand&Stare"]["Enticer"]["fadein"].asFloat();
        
        // Projector Settings
        c.projectorSerialName = configFile["Stand&Stare"]["Projector"]["name"].asString();
        c.projectorBaudRate = configFile["Stand&Stare"]["Projector"]["baud"].asInt();
        
        // Projector Settings
        c.arduinoName = configFile["Stand&Stare"]["Arduino"]["name"].asString();
        c.rfidTIRPin = configFile["Stand&Stare"]["Arduino"]["tirpin"].asInt();
        c.donationPin1 = configFile["Stand&Stare"]["Arduino"]["donationpin1"].asInt();
        c.donationPin2 = configFile["Stand&Stare"]["Arduino"]["donationpin2"].asInt();
        c.ledPin1 = configFile["Stand&Stare"]["Arduino"]["ledpin1"].asInt();
        c.ledPin2 = configFile["Stand&Stare"]["Arduino"]["ledpin2"].asInt();
        
        // Mask points
        vector<ofPoint> maskPoints;
        int numberOfMaskPoints = configFile["Stand&Stare"]["MaskPoints"].size();
        for (int maskPts = 0; maskPts < numberOfMaskPoints; maskPts++) {
            maskPoints.push_back(ofPoint(configFile["Stand&Stare"]["MaskPoints"][maskPts]["x"].asInt(),configFile["Stand&Stare"]["MaskPoints"][maskPts]["y"].asInt()));
        }
        c.maskPoints = maskPoints;
    }
}
//--------------------------------------------------------------
void LoadSettings::loadVideoConfig(string loadFile)
{
    d.clear();
    if (!configFile.open(loadFile)) {
        // Oh No!!!
        ofLog(OF_LOG_FATAL_ERROR,"Could not find the Video Configuration File");
        return;
    }
    else {
        // We're good lets get that config!!!
        ofLog(OF_LOG_NOTICE,"Found the Video Configuration File");

        // Video Settings
        int numberOfVideos = configFile["VideoData"]["videoslist"].size();
        for (int video = 0; video < numberOfVideos; video++) {
            VideoData vd;
            vd.id = configFile["VideoData"]["videoslist"][video]["id"].asInt();
            vd.videoUrl = configFile["VideoData"]["videoslist"][video]["videourl"].asString();
            vd.videoLength = configFile["VideoData"]["videoslist"][video]["videolength"].asString();
            vd.videoDetails = configFile["VideoData"]["videoslist"][video]["videodetails"].asString();

            vd.RFIDkey = configFile["VideoData"]["videoslist"][video]["rfidkey"].asString();
            vd.RFIDIcon = configFile["VideoData"]["videoslist"][video]["rfidicon"].asString();
            d.push_back(vd);
        }
    }
}
//--------------------------------------------------------------
Config LoadSettings::getConfig()
{
    return c;
}
//--------------------------------------------------------------
deque <VideoData> LoadSettings::getVideoConfig()
{
    return d;
}
//--------------------------------------------------------------
void LoadSettings::printConfiguration()
{
    cout << configFile.getRawString() << endl;
}