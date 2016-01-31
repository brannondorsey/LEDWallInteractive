//
//  FFTVisualizer.h
//  LEDWallVideoPlayer
//
//  Created by Brannon Dorsey on 1/28/16.
//
//

#ifndef __LEDWallVideoPlayer__FFTVisualizer__
#define __LEDWallVideoPlayer__FFTVisualizer__

#include "WallAppBase.h"
#include "ofxProcessFFT.h"

class FFTVisualizerApp : public WallAppBase {
public:

    FFTVisualizerApp();
    ~FFTVisualizerApp();
    void setup(ofFbo* fbo_);
    void update();
    void draw(int x, int y, int width, int height);
    void onAppSwitch();
    
    float shapeHue;
    float hueInterval;
    float verticalInterval;
    ofColor shapeColor;
    ofColor bg;
    ofxProcessFFT fft;
    
protected:
    void _drawBins();
    void _drawShape(int x, int y, int width, int height);
};

#endif /* defined(__LEDWallVideoPlayer__FFTVisualizer__) */
