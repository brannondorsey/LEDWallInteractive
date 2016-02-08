
#pragma once
#include "WallAppBase.h"
#include "OpenCVEngine.h"
#include "CloudBackground.h"
#include "ColorManager.h"


class OpenCVBurstApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
    void onAppSwitch();
   
    bool doUpdateBackground;
    
    int blockSize;
    int blockDrawSize;
    
    ofTexture dotTexture;
    void drawBackground();
    
    int blobCounter;
    
    CloudBackground cloudBackground;
    
    void drawBlobScaled(vector<ofPolyline>& blobs, int i, ofColor& color, ofVec3f center);
    
    int numBlobsToDraw;
    int drawCounter;
    vector<ofPolyline> blobs;
    pair<ofColor, ofColor> colorPair;
    ColorManager colorManager;
    
};
