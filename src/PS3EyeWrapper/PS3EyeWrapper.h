#pragma once

#include "ofMain.h"
#include "ps3eye.h"
#include "PS3EyeRunner.h"

class PS3EyeWrapper
{
public:
    PS3EyeWrapper();
    bool setup(int cameraWidth_, int cameraHeight_, int cameraFPS_, int id_);
    void update();
    void draw();
    void draw(int x, int y, int w, int h);
    void draw(int x, int y);

    ps3eye::PS3EYECam::PS3EYERef eye;
    ofTexture videoTexture;
    unsigned char * videoFrame;
    static void yuv422_to_rgba(const uint8_t *yuv_src, const int stride, uint8_t *dst, const int width, const int height);
    
    int width;
    int height;
    int fps;
    int id;
    
    int getWidth();
    int getHeight();
    int getFPS();
    
    ofPixels myPixels;

};