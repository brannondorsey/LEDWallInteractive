
#pragma once
#include "WallAppBase.h"
#include "TrackedWall.h"
#include "ofxTweenLite.h"

class OpenCVRingApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
    
    vector<TrackedWall*> trackedWalls;
   
    bool doUpdateBackground;
        
    
    int blockSize;
    int blockDrawSize;
    
    
    ofFbo contentLayerFBO;

    int canvasWidth;
    int canvasHeight;
    int videoSourceWidth;
    int videoSourceHeight;

    ofTexture dotTexture;
    ofVboMesh backgroundMesh;
    void drawBackground();
    void drawDebug();
    
    void drawRing(ofPoint center);
    float dx, dy, dz;
 


};
