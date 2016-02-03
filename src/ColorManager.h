//
//  ColorManager.h
//  ContourSilhouette
//
//  Created by bdorse on 1/20/16.
//
//

#ifndef __ContourSilhouette__ColorManager__
#define __ContourSilhouette__ColorManager__
#include "ofMain.h"

class ColorManager {
    
public:
    
    enum Mode {
        COLOR_MODE_HUE_SHIFT,
        COLOR_MODE_COMPLEMENTARY
    };
    
    ColorManager();
    
    void update();
    
    void setBackground(ofColor color);
    ofColor getBackground() const;
    ofColor getForeground() const;
    Mode getMode() const;
    void setMode(Mode mode);
    void setHueInterval(float interval);
    
protected:
    
    float _fgHue;
    int _fgHueDirection;
    float _fgHueBoundUpper;
    float _fgHueBoundLower;
    float _hueInterval;
    ofColor _bgColor;
    ofColor _fgColor;
    Mode _mode;
    
};

#endif /* defined(__ContourSilhouette__ColorManager__) */
