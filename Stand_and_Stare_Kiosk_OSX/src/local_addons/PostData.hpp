//
//  PostData.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#ifndef PostData_hpp
#define PostData_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxHttpUtils.h"

class PostData {
    
public:
    void setup(string postServer,string postExtension,string secretKey);
    void close();
    void postSystemUpdate(string status,string bulbtime);
    void postVideo(string count,string whichVideo,int playpercentage,bool didFinishVideo);
    void postDonation(int whichSlot);
    void newResponse(ofxHttpResponse & response);
    string getResponse();
    
    string getDebug();
    
private:
    string _postServer;
    string _postExtension;
    string _secretKey;
    string _responseStr;
    string _requestStr;
    ofxHttpUtils postUtils;
    stringstream debugData;
};

#endif /* PostData_hpp */
