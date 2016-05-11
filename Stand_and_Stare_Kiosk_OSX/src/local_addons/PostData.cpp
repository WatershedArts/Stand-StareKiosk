//
//  PostData.cpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 26/11/2015.
//
//

#include "PostData.h"
//--------------------------------------------------------------
void PostData::setup(string postServer, string postExtension, string secretKey)
{
    _postServer = postServer;
    _postExtension = postExtension;
    _secretKey = secretKey;
    _responseStr = "";
    
    ofAddListener(postUtils.newResponseEvent, this, &PostData::newResponse);
    postUtils.setMaxRetries(3);
    postUtils.start();
    debugData.clear();
}
//--------------------------------------------------------------
void PostData::close()
{
    ofRemoveListener(postUtils.newResponseEvent, this, &PostData::newResponse);
    postUtils.stop();
}
//--------------------------------------------------------------
void PostData::postSystemUpdate(string status,string bulbtime)
{
    debugData.str("");
    ofxHttpForm formIn;
    formIn.action = _postServer +_postExtension;
    formIn.method = OFX_HTTP_POST;
    formIn.addFormField("secret", _secretKey);
    formIn.addFormField("unit", status);
    formIn.addFormField("bulbtime", ofToString(bulbtime));
    formIn.addFormField("temperature",ofToString(ofSystem("/opt/vc/bin/vcgencmd measure_temp")));
    formIn.addFormField("status","1");
    postUtils.addForm(formIn);
    debugData << _postServer +"/"+_postExtension << endl;
    debugData << status << endl;
}
//--------------------------------------------------------------
void PostData::postVideo(string count,string whichVideo,int playpercentage,bool didFinishVideo)
{
    debugData.str("");
    ofxHttpForm formIn;
    formIn.action = _postServer +_postExtension;
    formIn.method = OFX_HTTP_POST;
    formIn.addFormField("secret", _secretKey);
    formIn.addFormField("videoselection", whichVideo);
    formIn.addFormField("playpercentage", ofToString(playpercentage));
    formIn.addFormField("didfinishvideo",ofToString(didFinishVideo));
    formIn.addFormField("count", "1");
    string _timestamp = ofGetTimestampString("%Y-%m-%d %H:%M:%s");
    formIn.addFormField("rawtimestamp", _timestamp);
    formIn.addFormField("video","1");
    postUtils.addForm(formIn);
    debugData << _postServer +"/"+_postExtension << endl;
    debugData << whichVideo << endl;
    debugData << _timestamp << endl;
    debugData << didFinishVideo << endl;
}
//--------------------------------------------------------------
void PostData::postDonation(int whichSlot)
{
    debugData.str("");
    ofxHttpForm formIn;
    formIn.action = _postServer +_postExtension;
    formIn.method = OFX_HTTP_POST;
    formIn.addFormField("secret", _secretKey);
    formIn.addFormField("count", "1");
    formIn.addFormField("slot", ofToString(whichSlot));
    string _timestamp = ofGetTimestampString("%Y-%m-%d %H:%M:%s");
    formIn.addFormField("rawtimestamp", _timestamp);
    formIn.addFormField("donation","1");
    debugData  << _postServer +"/"+_postExtension << endl;
    debugData << _timestamp << endl;
    postUtils.addForm(formIn);
}
//--------------------------------------------------------------
string PostData::getResponse()
{
    return _responseStr;
}
//--------------------------------------------------------------
string PostData::getDebug()
{
    return debugData.str();
}
//--------------------------------------------------------------
void PostData::newResponse(ofxHttpResponse &response)
{
    _responseStr = ofToString(response.status) + ":" + (string)response.responseBody;
    debugData << _responseStr << endl;
    debugData << "|------------------------------" << endl;
}
