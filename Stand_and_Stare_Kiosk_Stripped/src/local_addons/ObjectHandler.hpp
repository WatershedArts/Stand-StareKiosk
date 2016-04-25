//
//  ObjectHandler.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 16/12/2015.
//
//

#ifndef ObjectHandler_hpp
#define ObjectHandler_hpp

#include <stdio.h>
#include "ofMain.h"

class ObjectHandler {

public:
    void loadObjects(vector<string> images);
    void drawObjects();
private:
    vector<ofImage> images;
    vector<ofRectangle> imagePlacements;
protected:
    
    
};
#endif /* ObjectHandler_hpp */
