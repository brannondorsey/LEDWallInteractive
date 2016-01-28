
#pragma once
#include "WallAppBase.h"
#include "OpenCVEngine.h"



class MinimalApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
    
    
    bool doUpdateBackground;
    
    
    int blockSize;
    int blockDrawSize;
    
    ofTexture dotTexture;
    ofVboMesh backgroundMesh;
    void drawBackground();
    
    int blobCounter;
    
    void onAppSwitch(){};
    
    ofTexture texture;
    
};
