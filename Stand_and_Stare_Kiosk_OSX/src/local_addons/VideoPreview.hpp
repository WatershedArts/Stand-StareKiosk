//
//  VideoPreview.hpp
//  Stand_StareKiosk
//
//  Created by David Haylock on 12/01/2016.
//
//

#ifndef VideoPreview_hpp
#define VideoPreview_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxThreadedGifLoader.h"
#include "LoadSettings.hpp"
#include "SimpleButton.h"

#define GIF_WIDTH 160
#define GIF_HEIGHT 90
#define GIF_SPACING 10 

class VideoPreview {
    
    public:
        void setup(deque <SSVideoData> files);
        void update();
        void draw();
        void close();
        ofxThreadedGifLoader loader;
        void gifLoaded(ofxGifLoadedEvent &e);


        deque <SimpleButton> buttons;
        deque <ofVec2f> btnPts;
        SimpleButton saveChanges;
        SimpleButton discardChanges;
        deque <SSVideoData> _files;
    
        void mouseOver(int x, int y);
        void mousePressed(int x, int y);
        bool loaded;
        int totalWidth;
        int setButtons;
};

#endif /* VideoPreview_hpp */
