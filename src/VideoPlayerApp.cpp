//
//  VideoPlayerApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "VideoPlayerApp.h"


void VideoPlayerApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    loadRandomVideo();
}


void VideoPlayerApp::loadRandomVideo()
{
    string videoPath = "";
    ofDirectory currentVideoDirectory(ofToDataPath("videos", true));
    if (currentVideoDirectory.exists())
    {
        currentVideoDirectory.listDir();
        vector<ofFile> files = currentVideoDirectory.getFiles();
        if(files.size()>0)
        {
            if(files.size()==1)
            {
                videoPath = files[0].path();
            }else
            {
                videoPath = files[ofRandom(files.size())].path();
            }
        }
    }
    loadVideo(videoPath);
    
}

void VideoPlayerApp::onAppSwitch()
{
#if 0
    ofLogVerbose(__func__) << "VideoPlayerApp";
    
    fbo->begin();
    ofClear(0, 0, 0, 0);
    fbo->end();
#endif
    loadRandomVideo();
}
void VideoPlayerApp::update()
{
    fbo->begin();
        player.update();
        player.draw(0, 0, getWidth(), getHeight());
    fbo->end();
}


void VideoPlayerApp::loadVideo(string videoPath)
{
    
    player.loadMovie(videoPath);
    player.setLoopState(OF_LOOP_NORMAL);
    player.play();
    
}