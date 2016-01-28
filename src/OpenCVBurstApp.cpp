//
//  OpenCVBurstApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "OpenCVBurstApp.h"

bool doParticles = false;
void OpenCVBurstApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    doUpdateBackground = true;
    
    OpenCVEngine::getInstance().setup();
    
    blockSize = 10;
    blockDrawSize = 10;

    blobCounter = 0;

    ofLoadImage(dotTexture, "dot.png");
    cloudBackground.setup(ofGetWidth(), ofGetHeight());
    
    numBlobsToDraw = 0;


}

void OpenCVBurstApp::drawBlobScaled(vector<ofPolyline>& blobs, int i, ofColor& color, ofVec3f center)
{
    
    //if(blobs.empty()) return;
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    int alphaStep = 255/blobs.size();
    ofPushMatrix();
    ofPushStyle();
    ofColor wAlpha(color, i*alphaStep);
    ofSetColor(wAlpha);
    ofTranslate(center);
    ofBeginShape();
    for (int j=0; j<blobs[i].getVertices().size(); j++)
    {
        ofVertex(blobs[i].getVertices()[j]);
    }
    ofEndShape();
    ofPopMatrix();
    ofPopStyle();
    ofDisableBlendMode();
}

void drawBlobAsPixels(ofPolyline& blob, ofColor& color, int w, int h)
{
    if(blob.getVertices().empty()) return;
    

    ofPushStyle();
    int step = 16;
    for (int x = 0; x<w; x+=step)
    {
        for (int y = 0; y<h; y+=step)
        {
            bool isPointInside = ofPolyline::inside(x, y, blob);
            if(isPointInside)
            {
                
                //drawGradient(ofColor::red, ofColor::yellow, OF_GRADIENT_CIRCULAR, blockDrawSize, blockDrawSize);
                ofSetColor(color);
                ofRect(x, y, 12, 12);
            }
        }
    }
    ofPopStyle();
}




void OpenCVBurstApp::update()
{

    cloudBackground.update();
    OpenCVEngine::getInstance().update();
    

    vector<TrackedWall*>& trackedWalls = OpenCVEngine::getInstance().trackedWalls;
  
    //Draw at full res
    OpenCVEngine::getInstance().contentLayerFBO.begin();

    
    
    ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            cloudBackground.draw();
        ofDisableBlendMode();
    ofPopStyle();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofColor color;
    
    for (int i=0; i<trackedWalls.size(); i++)
    {
        TrackedWall* currentWall = trackedWalls[i];
        ofTranslate(i*OpenCVEngine::getInstance().videoSourceWidth, 0);
        currentWall->createBlobs(5);
        for (int j=0; j<currentWall->blobs.size(); j++)
        {
            
            drawCounter = 1;
            currentWall->fbo.begin();
            ofColor orange(255, 137, 87);
            ofColor purple(114, 0, 255);

            if(i == 0)
            {
                color = ofColor::red;
                
                
            }else
            {
                color = purple;
                
                
            }
            
            currentWall->drawBlobScaled(j, color);
            currentWall->mainBlob.draw();
            //drawBlobAsPixels(currentWall->mainBlob, color, 640, 480);
            
            currentWall->drawMainBlobPixels(color);
            
            ofPopMatrix();
            
            currentWall->fbo.end();
            
        }
        
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        currentWall->fbo.draw(0, 0);
        ofDisableBlendMode();
        currentWall->blobs.clear();
        
        
        
    }
    ofPopStyle();
    ofPopMatrix();

    OpenCVEngine::getInstance().contentLayerFBO.end();
    

    //
    fbo->begin();

    ofPushStyle();
        //drawBackground();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        OpenCVEngine::getInstance().contentLayerFBO.draw(0,
                             0,
                             480,
                             120);
    ofDisableBlendMode();
    ofPopStyle();
    fbo->end();
    
    

}

void OpenCVBurstApp::drawBackground()
{
    
    drawGradientMesh(ofColor::yellow,
                     ofColor::red,
                     OF_GRADIENT_BAR,
                     ofGetWidth(),
                     ofGetHeight());
}




