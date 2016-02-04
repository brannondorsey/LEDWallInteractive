//
//  ColorSchemes.h
//  LEDWallVideoPlayer
//
//  Created by bdorse on 2/4/16.
//
//

#ifndef __LEDWallVideoPlayer__ColorSchemes__
#define __LEDWallVideoPlayer__ColorSchemes__

#include "ofMain.h"

class ColorSchemes {
public:
    void setup();
    void loadNew();
    ofColor getColor(int index); // index = 0 - 4
    std::vector<ofColor> getColors();
    
protected:
    
    std::vector<ofColor> _colors;
    std::vector<std::vector<std::string> > _schemes;
};

#endif /* defined(__LEDWallVideoPlayer__ColorSchemes__) */
