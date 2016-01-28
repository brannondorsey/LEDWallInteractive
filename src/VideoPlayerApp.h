#pragma once
#include "WallAppBase.h"



class VideoPlayerApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    
    void loadVideo(string videoPath);
    void update();
    ofVideoPlayer player;

    void loadRandomVideo();
    void onAppSwitch();
    
};
