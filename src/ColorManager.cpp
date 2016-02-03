//
//  ColorManager.cpp
//  ContourSilhouette
//
//  Created by bdorse on 1/20/16.
//
//

#include "ColorManager.h"
ColorManager::ColorManager():
_fgHue(0),
_fgHueDirection(1),
_hueInterval(5),
_mode(COLOR_MODE_COMPLEMENTARY)
{
}

void ColorManager::update() {
    
    if (_mode == COLOR_MODE_COMPLEMENTARY) {
        
        if (_fgHueBoundUpper > _fgHueBoundLower) {
            if (_fgHueDirection > 0) {
                if (_fgHue + _hueInterval > _fgHueBoundUpper) {
                    _fgHueDirection = -_fgHueDirection;
                }
            } else {
                if (_fgHue - _hueInterval < _fgHueBoundLower) {
                    _fgHueDirection = -_fgHueDirection;
                }
            }
        } else {
            if (_fgHueDirection > 0) {
                if (_fgHue + _hueInterval > _fgHueBoundLower) {
                    _fgHueDirection = -_fgHueDirection;
                }
            } else {
                if (_fgHue - _hueInterval < _fgHueBoundUpper) {
                    _fgHueDirection = -_fgHueDirection;
                }
            }
        }
        
        _fgHue += _hueInterval * _fgHueDirection;
        
    } else if (_mode == COLOR_MODE_HUE_SHIFT) {
        
        if (_fgHue + _hueInterval < 360) {
            _fgHue += _hueInterval;
        } else {
            _fgHue = 0;
        }
    }
    
    _fgColor.setHueAngle(_fgHue);
}

void ColorManager::setBackground(ofColor color) {
    _bgColor = color;
    _fgColor = color;
    if (_mode == COLOR_MODE_COMPLEMENTARY) {
        float boundLimit = 35;
        _fgHue = abs(int(_bgColor.getHueAngle()) - 180);
        _fgColor.setHueAngle(_fgHue);
        _fgHueBoundLower = int(_fgHue - boundLimit) % 360;
        _fgHueBoundUpper = int(_fgHue + boundLimit) % 360;
        ofLogNotice() << "complementary: " << _fgHue << " upper bound: "
        << _fgHueBoundUpper << " lower bound: " << _fgHueBoundLower;
    }
}

ofColor ColorManager::getBackground() const {
    return _bgColor;
}


ofColor ColorManager::getForeground() const {
    return _fgColor;
}


ColorManager::Mode ColorManager::getMode() const {
    return _mode;
}


void ColorManager::setMode(Mode mode) {
    _mode = mode;
}

void ColorManager::setHueInterval(float interval) {
    _hueInterval = interval;
}
