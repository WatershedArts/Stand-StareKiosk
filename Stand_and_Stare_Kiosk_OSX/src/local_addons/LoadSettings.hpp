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
struct VideoData {
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
        datastream << "Icon: " << RFIDIcon << endl;
        datastream << "RFIDKey: " << RFIDkey << endl;
        datastream << "Video Url: " << videoUrl << endl;
        datastream << videoDetails << endl;
        return datastream.str();
    }
};

// Structure for All Data
struct Config {
    
    // General Configuration
    string unitName;
    int unitId;
    float fadeInTime;
    float fadeOutTime;
    int ldrSensitivity;
    bool setFullscreen;
    bool useWarper;
    
    // Post Data Configuration
    string postHostURL;
    string postExtURL;
    string secretKey;
    
    // Serial Configuration
    string RFIDSerialName;
    int rfidBaud;
    int rfidDelay;
    
    string projectorSerialName;
    int projectorBaudRate;
    bool useProjectorController;
    
    // Enticer
    string enticerVideoUrl;
    float enticerFadeOut;
    float enticerFadeIn;
    
    // Donation Reader
    string DonationReaderName;
    int donationsBaud;
    
    // Arduino
    string arduinoName;
    int rfidTIRPin;
    int donationPin1;
    int donationPin2;
    
    vector <ofPoint> maskPoints;
};

class LoadSettings {
    public:
    
        //! Load the Settings File
        void loadConfig(string loadFile);

        //! Load the VideoData File
        void loadVideoConfig(string loadFile);
    
        //! Return the Config Values
        Config getConfig();
    
        //! Return the Config Values
        deque <VideoData> getVideoConfig();
    
        //! Print out the Raw Json String
        void printConfiguration();
    
    private:
        Config c;
        deque<VideoData> d;
    protected:
        ofxJSONElement configFile;
};
#endif /* LoadSettings_hpp */
