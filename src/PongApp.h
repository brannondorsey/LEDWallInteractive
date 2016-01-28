
#pragma once
#include "WallAppBase.h"
#include "OpenCVEngine.h"
#include "PongGame.h"


struct PlayerWall
{
    TrackedWall* wall;
    int id;
    PlayerWall()
    {
        id = -1;
        wall = NULL;
        
    }
};

class PongApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
    
    int blobCounter;
    
    PlayerWall leftWall;
    PlayerWall rightWall;
    vector<PlayerWall*> playerWalls;
    
    int singleWallWidth;
    void onAppSwitch();
    
    PongGame game;
};
