//
//  ColorSchemes.cpp
//  LEDWallVideoPlayer
//
//  Created by bdorse on 2/4/16.
//
//

#include "ColorSchemes.h"

void ColorSchemes::setup() {
    ofBuffer buff = ofBufferFromFile("colors.txt");
    std::string file = ofToString(buff);
    std::vector<std::string> schemes = ofSplitString(file, "\n", true);
    for (int i = 0; i < schemes.size(); i++) {
        _schemes.push_back(ofSplitString(schemes[i], "-"));
    }
    
    loadNew();
};

void ColorSchemes::loadNew() {
    _colors.clear();
    int index = (int) ofRandom(0, _schemes.size() - 1);
    std::vector<std::string> scheme = _schemes[index];
    for (int i = 0; i < scheme.size(); i++) {
        ofColor color = color.fromHex(ofHexToInt("0x" + scheme[i]));
        _colors.push_back(color);
    }
};

ofColor ColorSchemes::getColor(int index) {
    ofColor color;
    if (index > -1 && index < _colors.size()) {
        color = _colors[index];
    }
    return color;
};

std::vector<ofColor> ColorSchemes::getColors() {
    return _colors;
};
