//
//  PS3EyeWrapper.cpp
//  emptyExample
//
//  Created by Jason Van Cleave on 9/24/15.
//
//

#include "PS3EyeWrapper.h"

int ITUR_BT_601_CY = 1220542;
int ITUR_BT_601_CUB = 2116026;
int ITUR_BT_601_CUG = -409993;
int ITUR_BT_601_CVG = -852492;
int ITUR_BT_601_CVR = 1673527;
int ITUR_BT_601_SHIFT = 20;

void PS3EyeWrapper::yuv422_to_rgba(const uint8_t *yuv_src, const int stride, uint8_t *dst, const int width, const int height)
{
    const int bIdx = 2;
    const int uIdx = 0;
    const int yIdx = 0;
    
    const int uidx = 1 - yIdx + uIdx * 2;
    const int vidx = (2 + uidx) % 4;
    int j, i;
    
#define _max(a, b) (((a) > (b)) ? (a) : (b))
#define _saturate(v) static_cast<uint8_t>(static_cast<uint32_t>(v) <= 0xff ? v : v > 0 ? 0xff : 0)
    
    for (j = 0; j < height; j++, yuv_src += stride)
    {
        uint8_t* row = dst + (width * 4) * j; // 4 channels
        
        for (i = 0; i < 2 * width; i += 4, row += 8)
        {
            int u = static_cast<int>(yuv_src[i + uidx]) - 128;
            int v = static_cast<int>(yuv_src[i + vidx]) - 128;
            
            int ruv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVR * v;
            int guv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVG * v + ITUR_BT_601_CUG * u;
            int buv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CUB * u;
            
            int y00 = _max(0, static_cast<int>(yuv_src[i + yIdx]) - 16) * ITUR_BT_601_CY;
            row[2-bIdx] = _saturate((y00 + ruv) >> ITUR_BT_601_SHIFT);
            row[1]      = _saturate((y00 + guv) >> ITUR_BT_601_SHIFT);
            row[bIdx]   = _saturate((y00 + buv) >> ITUR_BT_601_SHIFT);
            row[3]      = (0xff);
            
            int y01 = _max(0, static_cast<int>(yuv_src[i + yIdx + 2]) - 16) * ITUR_BT_601_CY;
            row[6-bIdx] = _saturate((y01 + ruv) >> ITUR_BT_601_SHIFT);
            row[5]      = _saturate((y01 + guv) >> ITUR_BT_601_SHIFT);
            row[4+bIdx] = _saturate((y01 + buv) >> ITUR_BT_601_SHIFT);
            row[7]      = (0xff);
        }
    }
}

PS3EyeWrapper::PS3EyeWrapper()
{
    id = -1;
}

bool PS3EyeWrapper::setup(int width_, int height_, int fps_, int id_)
{
    std::vector<ps3eye::PS3EYECam::PS3EYERef> devices( ps3eye::PS3EYECam::getDevices() );

    bool didInit = false;
    id = id_;
    if (devices.empty())
    {
        ofLogVerbose(__func__) << "PS3 ID" << id << " FAILED";
        return didInit;
    }
    eye = devices.at(id);
    width = width_;
    height = height_;
    fps = fps_;
    
    didInit = eye->init(width, height, fps);
    
    if (didInit)
    {
        eye->start();
        videoFrame 	= new unsigned char[width*height*4];
        videoTexture.allocate(width, height, GL_RGBA);
        //myPixels.setFromExternalPixels(videoFrame, width, height, 4);
    }
    return didInit;
}
int PS3EyeWrapper::getWidth()
{
    return width;
}

int PS3EyeWrapper::getHeight()
{
    return height;
}

int PS3EyeWrapper::getFPS()
{
    return fps;
}

void PS3EyeWrapper::update()
{
    ps3eye::PS3EYECam::updateDevices();
    if(eye)
    {
        bool isNewFrame = eye->isNewFrame();
        if(isNewFrame)
        {
            yuv422_to_rgba(eye->getLastFramePointer(), eye->getRowBytes(), videoFrame, width, height);
            videoTexture.loadData(videoFrame, width, height, GL_RGBA);
            //myPixels.setFromExternalPixels(videoFrame, width, height, 4);

        }
    }
 
}
void PS3EyeWrapper::draw()
{
    draw(0, 0, width, height);
}

void PS3EyeWrapper::draw(int x, int y, int w, int h)
{
    videoTexture.draw(x, y, w, h);
}
void PS3EyeWrapper::draw(int x, int y)
{
  
    videoTexture.draw(x, y, width, height);

}
