//
//  LoadSettings.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef LoadSettings_hpp
#define LoadSettings_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxJSON.h"

struct SSAudioData {
    int id;
    string audioUrl;
    float audioLength;
    string audioDetails;
    string RFIDkey;
    string RFIDIcon;
    
    string getData() {
        stringstream datastream;
        datastream << "|-----------------------------" << endl;
        datastream << "| Icon: " << RFIDIcon << endl;
        datastream << "| RFIDKey " << RFIDkey << endl;
        datastream << "| Audio Url " << audioUrl << endl;
        datastream << "| " << audioDetails << endl;
        return datastream.str();
    }
};

struct Config {
    
    // General Configuration
    float fadeInTime;
    float fadeOutTime;
    
    // Post Data Configuration
    string postHostURL;
    string postExtURL;
    string secretKey;
    
    // Video Data Configuration
    deque <SSAudioData> audios;
    
    // Serial Configuration
    string RFIDSerialName;
    int rfidBaud;
    int rfidDelay;
    
    // Enticer
    string enticerAudioUrl;
    float enticerFadeOut;
    float enticerFadeIn;
    float enticerAudioLength;
};

class LoadSettings {
public:
    void load(string loadFile);
    Config getConfig();
    
    void printConfiguration();
    
private:
    Config c;
protected:
     ofxJSONElement configFile;
};
#endif /* LoadSettings_hpp */
