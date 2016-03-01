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


// Structure for Video Data
struct SSVideoData {
    int id;
    string videoUrl;
    string videoLength;
    string videoDetails;
    string RFIDkey;
    string RFIDIcon;
    
    // Quad Warping
    vector<ofPoint> quadWarpingPoints;
    
    string getData() {
        stringstream datastream;
        datastream << "|-----------------------------" << endl;
        datastream << "| Icon: " << RFIDIcon << endl;
        datastream << "| RFIDKey " << RFIDkey << endl;
        datastream << "| Video Url " << videoUrl << endl;
        datastream << "| " << videoDetails << endl;
        return datastream.str();
    }
};

// Structure for All Data
struct Config {
    
    // General Configuration
    float fadeInTime;
    float fadeOutTime;
    int ldrSensitivity;
    bool setFullscreen;
    bool useWarper;
    
    // Post Data Configuration
    string postHostURL;
    string postExtURL;
    string secretKey;
    
    // Video Data Configuration
    deque <SSVideoData> videos;
    
    // Serial Configuration
    string RFIDSerialName;
    int rfidBaud;
    int rfidDelay;
    
    string projectorSerialName;
    bool useProjectorController;
    
    // Enticer
    string enticerVideoUrl;
    float enticerFadeOut;
    float enticerFadeIn;
    
    // Donation Reader
    string DonationReaderName;
    int donationsBaud;
    
    vector <ofPoint> maskPoints;
};

class LoadSettings {
    public:
    
        //! Load the Settings Folder
        void load(string loadFile);
        
        //! Return the values
        Config getConfig();
        
        //! Print out the Raw Json String
        void printConfiguration();
    
    private:
        Config c;
    protected:
        ofxJSONElement configFile;
};
#endif /* LoadSettings_hpp */
