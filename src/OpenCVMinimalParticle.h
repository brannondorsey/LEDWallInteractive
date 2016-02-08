
#pragma once
#include "WallAppBase.h"
#include "OpenCVEngine.h"

#include "ParticleLayer.h"
#include "OpenCVParticleApp.h"

struct WallParticleGroup
{
    vector<ofxParticle*> particles;
    TrackedWall* wall;
    int id;
    WallParticleGroup()
    {
        id = -1;
        wall = NULL;
        
    }
};

class OpenCVMinimalParticle : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
   
    bool doUpdateBackground;
        
    
    
    ParticleLayer* particleLayer;
    int particleSize;
    void updateParticles();
    
    ofFbo backgroundFBO;
    ofFbo contentFBO;

    ofTexture particleTexture;
    void drawBackground();
        
    WallParticleGroup leftWall;
    WallParticleGroup rightWall;
    vector<WallParticleGroup*> wallGroups;
    vector<ColorPair> colorPairs;
    int currentColorPairIndex;
    
    void onAppSwitch();
    
    int singleWallWidth;
    int wholeWallWidth;
};
