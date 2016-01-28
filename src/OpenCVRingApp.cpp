//
//  OpenCVRingApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "OpenCVRingApp.h"
ofImage image;

void OpenCVRingApp::setup(ofFbo* fbo_)
{
    dy = dx = dz = 0;
    setFBO(fbo_);
    doUpdateBackground = true;
    
    TrackedWall* wall1 = new TrackedWall(0);
    wall1->setup("");
    trackedWalls.push_back(wall1);
    
    TrackedWall* wall2 = new TrackedWall(1);
    wall2->setup("");
    //wall2->videoSource.doRandom = true;
    //wall2->doUpdateBackground = false;
    wall2->videoSource.videoPlayer.setLoopState(OF_LOOP_PALINDROME);
    trackedWalls.push_back(wall2);
    videoSourceWidth = 640;
    videoSourceHeight = 480;
    
    canvasWidth = videoSourceWidth*2;
    canvasHeight = videoSourceHeight;
   
    contentLayerFBO.allocate(canvasWidth, canvasHeight);
    contentLayerFBO.begin();
        ofClear(0, 0, 0, 0);
    contentLayerFBO.end();
    
 
    blockSize = 10;
    blockDrawSize = 10;


    ofLoadImage(dotTexture, "dot.png");
    image.allocate(videoSourceWidth/2, videoSourceHeight/2, OF_IMAGE_COLOR_ALPHA);
    
 

}


void OpenCVRingApp::update()
{
    for (int i=0; i<trackedWalls.size(); i++)
    {
        trackedWalls[i]->update();
    }
    //Draw at full res
    contentLayerFBO.begin();
    
    
    
    

    

    
    //ofColor c1(getRandomColor());
    ofColor c1(ofColor::blueViolet);
    ofColor c2(ofColor::yellowGreen);
    //float progress = ofGetElapsedTimeMillis()%100;
    //ofLogVerbose() << "progress: " << progress;
    
   // float scale = ofxTweenLite::tween(1, 1000, progress, OF_EASE_LINEAR_IN );
   // float finalScale = ofMap(progress, 0, 100, 0.1, 1.0);
   // ofLogVerbose() << "finalScale: " << finalScale;

    
    if(!doDrawDebug)
    {
        for (int i=0; i<trackedWalls.size(); i++)
        {
            if(trackedWalls[i]->mainBlob.getCentroid2D() != ofVec3f::zero())
            {
                ofPushStyle();
                ofPushMatrix();
                ofTranslate(i*videoSourceWidth, 0);
                    trackedWalls[i]->fbo.begin();
                    //ofClear(0);
                    float radius = ofGetElapsedTimeMillis()%videoSourceHeight;
                    float halfRadius = radius*0.75;
                    ofBlendMode(OF_BLENDMODE_ALPHA);
                    ofSetColor(ofColor(ofColor::blue, ofRandom(128, 255)));
                    ofCircle(trackedWalls[i]->mainBlob.getCentroid2D(), radius);
                    ofSetColor(ofColor(ofColor::whiteSmoke, 128));
                    ofCircle(trackedWalls[i]->mainBlob.getCentroid2D(), halfRadius);
                    ofDisableBlendMode();
                    trackedWalls[i]->fbo.end();
                    trackedWalls[i]->fbo.draw(0, 0);
                ofPopMatrix();
                ofPopStyle();
                //
            }



            
        }
        
        
        
    }
    else
    {
        drawDebug();
    }
    contentLayerFBO.end();
    
    
    //
    fbo->begin();
    
    ofPushStyle();
        drawBackground();

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        contentLayerFBO.draw(0,
                             0,
                             480,
                             120);
        

    ofDisableBlendMode();
    ofPopStyle();

    fbo->end();
    
    

}

void OpenCVRingApp::drawBackground()
{
    if(backgroundMesh.getNumVertices()==0)
    {
        initGradientMesh(backgroundMesh,
                         ofColor::yellow,
                         ofColor::red,
                         OF_GRADIENT_BAR,
                         ofGetWidth(),
                         ofGetHeight());
    }else
    {
        ofEnableDepthTest();
        backgroundMesh.draw();
        ofDisableDepthTest();
    }
}


void OpenCVRingApp::drawDebug()
{
    for (int i=0; i<trackedWalls.size(); i++)
    {
        ofPushMatrix();
            ofTranslate(i*videoSourceWidth, 0);
            trackedWalls[i]->drawDebug();
            ofCircle(trackedWalls[i]->mainBlob.getCentroid2D(), 20);
        ofPopMatrix();

    }

}

float fractalNoise(float x, float y, float z) {
    float r = 0;
    float amp = 1.0;
    for (int octave = 0; octave < 4; octave++) {
        r += ofNoise(x, y, z) * amp;
        amp /= 2;
        x *= 2;
        y *= 2;
        z *= 2;
    }
    return r;
}




void  OpenCVRingApp::drawRing(ofPoint center)
{
    long now = ofGetElapsedTimeMillis();
    float speed = 0.002;
    float zspeed = 0.8;
    float angle = sin(now * 0.001);
    float z = now * 0.00008;
    float hue = now * 0.01;
    float scale = 0.0005;
    float saturation = 100 * ofClamp(pow(1.02 * ofNoise(now * 0.000122), 2.5), 0, 1);
    saturation = 0;
    float spacing = ofNoise(now * 0.000124) * 0.1;
    
    dx += cos(angle) * speed;
    dy += sin(angle) * speed;
    dz += -0.14; //(noise(now * 0.000014) - 0.5) * zspeed;

    int width  = image.getWidth();
    int height = image.getHeight();
    //float centerx = ofNoise(now *  0.000125) * 1.25 * width;
    //float centery = ofNoise(now * -0.000125) * 1.25 * height;
    float centerx = center.x;
    float centery = center.y;
   
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            
            float dist = sqrt(pow(x - centerx, 2) + pow(y - centery, 2));
            float pulse = (sin(dz + dist * spacing) - 0.8) * 3;
            
            float n = fractalNoise(dx + x*scale, dy + y*scale, z) - 0.29;
            float m = fractalNoise(dx + x*scale, dy + y*scale, z + 10.0) - 0.75;
            
            ofColor c(
                            0,
                            saturation,
                            100 * ofClamp(pulse * pow(3.0 * n, 1.5), 0, 0.9)
                            );
            //ofColor c(ofColor::blue);
            image.setColor(x, y, c);
        }
    }
    image.update();
   

}




