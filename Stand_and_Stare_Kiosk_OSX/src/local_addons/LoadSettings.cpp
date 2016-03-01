//
//  LoadSettings.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "LoadSettings.hpp"
//--------------------------------------------------------------
void LoadSettings::load(string loadFile)
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

        // Donations Settings
        c.DonationReaderName = configFile["Stand&Stare"]["Donation"]["name"].asString();
        c.donationsBaud = configFile["Stand&Stare"]["Donation"]["baud"].asInt();
        
        c.enticerVideoUrl = configFile["Stand&Stare"]["Enticer"]["videourl"].asString();
        c.enticerFadeOut = configFile["Stand&Stare"]["Enticer"]["fadeout"].asFloat();
        c.enticerFadeIn = configFile["Stand&Stare"]["Enticer"]["fadein"].asFloat();
        
        // Mask points
        vector<ofPoint> maskPoints;
        int numberOfMaskPoints = configFile["Stand&Stare"]["MaskPoints"].size();
        for (int maskPts = 0; maskPts < numberOfMaskPoints; maskPts++) {
            maskPoints.push_back(ofPoint(configFile["Stand&Stare"]["MaskPoints"][maskPts]["x"].asInt(),configFile["Stand&Stare"]["MaskPoints"][maskPts]["y"].asInt()));
        }
        c.maskPoints = maskPoints;
        
        // Video Settings
        int numberOfVideos = configFile["Stand&Stare"]["VideoData"]["videoslist"].size();
        for (int video = 0; video < numberOfVideos; video++) {
            SSVideoData vd;
            vd.id = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["id"].asInt();
            vd.videoUrl = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["videourl"].asString();
            vd.videoLength = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["videolength"].asString();
            vd.videoDetails = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["videodetails"].asString();
            
            vd.gifUrl = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["gifurl"].asString();
            
            vd.RFIDkey = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["rfidkey"].asString();
            vd.RFIDIcon = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["rfidicon"].asString();
            vd.RFIDIcon = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["rfidicon"].asString();
            
            // Get the Warping points
            int warpingPts = configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["quadwarping"].size();
            for (int warpPts = 0; warpPts < warpingPts; warpPts++) {
                ofPoint p = ofPoint(configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["quadwarping"][warpPts]["x"].asInt(),
                                    configFile["Stand&Stare"]["VideoData"]["videoslist"][video]["quadwarping"][warpPts]["y"].asInt());
                vd.quadWarpingPoints.push_back(p);
            }
            c.videos.push_back(vd);
        }
    }
}
//--------------------------------------------------------------
Config LoadSettings::getConfig()
{
    return c;
}
//--------------------------------------------------------------
void LoadSettings::printConfiguration()
{
    cout << configFile.getRawString() << endl;
}