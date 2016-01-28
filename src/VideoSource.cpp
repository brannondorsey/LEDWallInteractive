#include "VideoSource.h"

VideoSource::~VideoSource()
{
    PS3EyeRunner::getInstance().stop();

}
VideoSource::VideoSource()
{
    isCamera = false;
    doRandom = false;
}

void VideoSource::setup(int width, int height, int fps=30, int deviceID=0)
{

    std::vector<ps3eye::PS3EYECam::PS3EYERef> devices( ps3eye::PS3EYECam::getDevices() );

    bool didCameraStart = camera.setup(width, height, fps, deviceID);
    if (didCameraStart)
    {
        PS3EyeRunner::getInstance().start();
        isCamera = true;
    }else
    {
        ofLogVerbose(__func__) << "CAMERA FAILED TO START " << deviceID;
    }
   
    
}

void VideoSource::setup(string videoPath)
{
    videoPlayer.loadMovie(videoPath);
    videoPlayer.play();
    isCamera = false;
}

void VideoSource::update()
{
    if(isCamera)
    {
        camera.update();
    }else
    {

        if(doRandom && ofGetFrameNum()%100 == 0)
        {
            videoPlayer.setFrame(ofRandom(videoPlayer.getTotalNumFrames()));

        }
        videoPlayer.update();
    }
}

bool VideoSource::isFrameNew()
{
    bool isNew = false;
    if(isCamera)
    {
        isNew = true;
    }else
    {
        isNew = videoPlayer.isFrameNew();
    }
    return isNew;
}

unsigned char* VideoSource::getPixels()
{
    if(isCamera)
    {
        return camera.videoFrame;
    }
    //return NULL;
}
ofBaseHasPixels& VideoSource::getSource()
{
    if(isCamera)
    {
        //return camera.videoTexture;
    }else
    {
        return videoPlayer;
    }
}

void VideoSource::draw(int x, int y)
{
    if(isCamera)
    {
        camera.draw(x, y);
    }else
    {
        videoPlayer.draw(x, y);
    }
}

void VideoSource::draw(int x, int y, int width, int height)
{
    if(isCamera)
    {
        camera.draw(x, y, width, height);
    }else
    {
        videoPlayer.draw(x, y, width, height);
    }
}

int VideoSource::getWidth()
{
    int width = 0;
    if(isCamera)
    {
        width = camera.getWidth();
    }else
    {
        width = videoPlayer.getWidth();
    }
    return width;
}

int VideoSource::getHeight()
{
    int height = 0;
    if(isCamera)
    {
        height = camera.getHeight();
    }else
    {
        height = videoPlayer.getHeight();
    }
    return height;
}