//
//  ContourSillhouette.h
//  LEDWallVideoPlayer
//
//  Created by bdorse on 1/31/16.
//
//

#ifndef __LEDWallVideoPlayer__ContourSillhouette__
#define __LEDWallVideoPlayer__ContourSillhouette__

#include "ofMain.h"

class ContourSillhouette {
public:
    ContourSillhouette();
    ContourSillhouette(const ofPath& path, const ofPoint& centroid);
    ~ContourSillhouette();
    void update();
    void draw();
    void setColor(const ofColor& color);
    bool isDead();
    
    float alphaDecrement;
    float scaleIncrement;
    
protected:
    ofPath _path;
    ofPoint _origCentroid;
    ofColor _color;
    float _scaleSum;
};

#endif /* defined(__LEDWallVideoPlayer__ContourSillhouette__) */
