//
//  FluidDynamicsApp.h
//  LEDWallVideoPlayer
//
//  Created by bdorse on 2/2/16.
//
//

#ifndef __LEDWallVideoPlayer__FluidDynamicsApp__
#define __LEDWallVideoPlayer__FluidDynamicsApp__

#include "ofxFluid.h"
#include "WallAppBase.h"
#include "ColorManager.h"
#include "TrackedWall.h"
#include "OpenCVEngine.h"
#include "CloudBackground.h"

class FluidDynamicsApp : public WallAppBase {
public:

    FluidDynamicsApp();
    ~FluidDynamicsApp();
    
    void setup(ofFbo* fbo_);
    void update();
    void onAppSwitch();
    
protected:
    
    void _draw(); // called interally by update
    
    ofxFluid _fluid;
    ofFbo _largeFbo;
    ofShader _screenShader;
    ColorManager _colorManager;
    ofVec2f _oldM;
    std::vector<TrackedWall*> _walls;
    std::pair<ofPoint, ofPoint> _prevPoints;
    CloudBackground _cloudBackground;

};

#endif /* defined(__LEDWallVideoPlayer__FluidDynamicsApp__) */
