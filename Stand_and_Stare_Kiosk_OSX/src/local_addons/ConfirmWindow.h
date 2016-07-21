//
//  ConfirmWindow.h
//  Stand_and_Stare_Kiosk_OSX
//
//  Created by David Haylock on 11/05/2016.
//
//

#ifndef ConfirmWindow_h
#define ConfirmWindow_h
#pragma once
#include "ofxModal.h"

class ConfirmWindow : public ofxModalWindow{
    
public:
    
    ConfirmWindow(){
        setTitle("Set Tag Properties");
        addComponent(new ofxDatGuiTextInput("Tag Icon", "Tag Icon"))->setTheme(&guiTheme);
        addComponent(new ofxDatGuiTextInput("Film Name", "Film Name"))->setTheme(&guiTheme);
        addComponent(new ofxDatGuiTextInput("Film ID", "Film ID"))->setTheme(&guiTheme);
        addComponent(new ofxDatGuiLabel("Tag Identifier"))->setTheme(&guiTheme);
        addButton("Cancel");
        getButton(0)->setLabel("Confirm");
        getButton(0)->setLabelColor(mTheme->color.button.darkblue.label);
        getButton(0)->setBackgroundColors(mTheme->color.button.darkblue.background,
                                          mTheme->color.button.darkblue.backgroundOnMouseOver, mTheme->color.button.darkblue.backgroundOnMouseDown);
        getButton(0)->setBorder(mTheme->color.button.darkblue.border, 1);
        autoSize();
    }
    
private:
    
    ofxDatGuiThemeCharcoal guiTheme;
    
};

#endif /* ConfirmWindow_h */
