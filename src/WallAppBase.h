#pragma once

#include "ofMain.h"

class WallAppBase
{
public:
    
    WallAppBase();
    void setFBO(ofFbo* fbo_);
    virtual void setup(ofFbo* fbo_) = 0;
    virtual void update()=0;
    virtual void  onAppSwitch() = 0;
    void draw();
    void draw(int x, int y, int width, int height);

    ofFbo* fbo;
    ofPixels pixels;
    ofPixels& getPixelsRef();
    
    float getWidth();
    float getHeight();
    
    
    static ofColor getRandomColor();
    
    static void drawGradientMesh(const ofColor& start,
                             const ofColor& end,
                             ofGradientMode mode,
                             float w,
                             float h);
    
    static void drawLetters();
    
    string name;
};
