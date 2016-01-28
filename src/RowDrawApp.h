#pragma once
#include "WallAppBase.h"



class RowDrawApp : public WallAppBase
{
public:
    
    
    int currentX;
    int currentY;
    
    void setup(ofFbo* fbo_);
    void update();
    
    void onAppSwitch(){};
};
