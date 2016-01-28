//
//  MinimalApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "MinimalApp.h"

void MinimalApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    doUpdateBackground = true;
    OpenCVEngine::getInstance().setup();
    ofLoadImage(texture, "featheredBar.png");
    
}


bool arePointsSame(ofVec3f p1, ofVec3f p2)
{
    bool result = false;
    if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z)
    {
        result = true;
    }
    return result;
}

void MinimalApp::update()
{
    OpenCVEngine::getInstance().update();
    
    //Process each wall, draw content into wall FBOs
    
    for(int i=0; i<OpenCVEngine::getInstance().trackedWalls.size(); i++)
    {
        TrackedWall* currentWall = OpenCVEngine::getInstance().trackedWalls[i];
        bool isSame = arePointsSame(currentWall->mainBlob.getCentroid2D(),
                                    ofVec3f::zero());
       // ofLogVerbose() << "isSame: " << isSame;
#if 0
        if (!isSame)
        {


        }else
        {
            currentWall->fbo.begin();
                ofClear(0, 0, 0, 0);
            currentWall->fbo.end();
        }
#endif
        currentWall->fbo.begin();
        ofClear(0, 0, 0, 0);
        currentWall->videoSource->draw(0, 0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        //texture.draw(currentWall->mainBlob.getCentroid2D(), 80, 1000);
        ofPushStyle();
        ofSetColor(ofColor::red);

        ofRect(currentWall->mainBlob.getCentroid2D(), 80, 1000);
        ofPopStyle();
        ofSetRectMode(OF_RECTMODE_CORNER);
        currentWall->fbo.end();
    }

    
    
    //Draw at full res
    OpenCVEngine::getInstance().contentLayerFBO.begin();
        ofClear(0, 0, 0, 0);
        ofPushMatrix();
            for(int i=0; i<OpenCVEngine::getInstance().trackedWalls.size(); i++)
            {
                TrackedWall* currentWall = OpenCVEngine::getInstance().trackedWalls[i];
                ofTranslate(i*640, 0);
                currentWall->fbo.draw(0, 0);
                
            }
        ofPopMatrix();
    
    OpenCVEngine::getInstance().contentLayerFBO.end();
    
    fbo->begin();
        ofClear(0, 0, 0, 0);
        ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            OpenCVEngine::getInstance().contentLayerFBO.draw(0,
                                                             0,
                                                             480,
                                                             120);
            ofDisableBlendMode();
        ofPopStyle();
    fbo->end();
    
    
    
}

void MinimalApp::drawBackground()
{

}




