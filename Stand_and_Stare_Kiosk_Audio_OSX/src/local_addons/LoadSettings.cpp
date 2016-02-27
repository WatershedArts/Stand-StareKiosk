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
        
        // Post Settings
        c.postHostURL = configFile["Stand&Stare"]["PostData"]["posthost"].asString();
        c.postExtURL = configFile["Stand&Stare"]["PostData"]["postext"].asString();
        c.secretKey = configFile["Stand&Stare"]["PostData"]["secretkey"].asString();
        
        // RFID Settings
        c.RFIDSerialName = configFile["Stand&Stare"]["RFID"]["name"].asString();
        c.rfidBaud = configFile["Stand&Stare"]["RFID"]["baud"].asInt();
        
        c.enticerAudioUrl = configFile["Stand&Stare"]["Enticer"]["audiourl"].asString();
        c.enticerFadeOut = configFile["Stand&Stare"]["Enticer"]["fadeout"].asFloat();
        c.enticerFadeIn = configFile["Stand&Stare"]["Enticer"]["fadein"].asFloat();
        c.enticerAudioLength = configFile["Stand&Stare"]["Enticer"]["audiolength"].asFloat();
        
        // Audio Settings
        int numberOfAudio = configFile["Stand&Stare"]["AudioData"]["audiolist"].size();
        for (int audio = 0; audio < numberOfAudio; audio++) {
            SSAudioData ad;
            ad.id = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["id"].asInt();
            ad.audioUrl = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["audiourl"].asString();
            ad.audioLength = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["audiolength"].asString();
            ad.audioDetails = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["audiodetails"].asString();
            ad.RFIDkey = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["rfidkey"].asString();
            ad.RFIDIcon = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["rfidicon"].asString();
            ad.RFIDIcon = configFile["Stand&Stare"]["AudioData"]["audioslist"][audio]["rfidicon"].asString();
            c.audios.push_back(ad);
        }
        // Debug
        printConfiguration();
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