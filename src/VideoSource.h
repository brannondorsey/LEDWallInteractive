#pragma once

#include "ofMain.h"
#include "PS3EyeWrapper.h"

class VideoSource
{
public:
    PS3EyeWrapper camera;
    ofVideoPlayer  videoPlayer;
    bool isCamera;
    
    
    VideoSource();
    ~VideoSource();
    
    void setup(int width, int height, int fps, int deviceID);
    void setup(string videoPath);
    void update();
    bool isFrameNew();
    ofBaseHasPixels& getSource();
    void draw(int x, int y);
    void draw(int x, int y, int width, int height);
    int getWidth();
    int getHeight();
    
    bool doRandom;
    unsigned char*  getPixels();

};
