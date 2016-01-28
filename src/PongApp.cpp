//
//  PongApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "PongApp.h"

void PongApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    
    OpenCVEngine::getInstance().setup();

    game.setup(OpenCVEngine::getInstance().canvasWidth, OpenCVEngine::getInstance().canvasHeight);
    singleWallWidth = OpenCVEngine::getInstance().videoSourceWidth;
    
    leftWall.id = 0;
    leftWall.wall = OpenCVEngine::getInstance().getWall(0);
    
    rightWall.id = 1;
    rightWall.wall = OpenCVEngine::getInstance().getWall(1);
    
    playerWalls.push_back(&leftWall);
    playerWalls.push_back(&rightWall);

    
    blobCounter = 0;
    
}




void PongApp::update()
{
   
    
    OpenCVEngine::getInstance().update();
    
    ofRectangle offset(rightWall.wall->mainBlob.getBoundingBox());
    offset.x+=singleWallWidth;
    
    game.update(leftWall.wall->mainBlob.getBoundingBox(), offset);
 
    //Draw at full res
    OpenCVEngine::getInstance().contentLayerFBO.begin();
    ofClear(0, 0, 0, 0);
    
    ofPushStyle();
        game.draw();
    ofPopStyle();
    OpenCVEngine::getInstance().contentLayerFBO.end();
    
    fbo->begin();
        ofPushStyle();
        ofClear(0, 0, 0, 0);
        OpenCVEngine::getInstance().contentLayerFBO.draw(0,
                                                         0,
                                                         480,
                                                         120);
        ofPopStyle();
    
    fbo->end();
    
    

}
void PongApp::onAppSwitch()
{

}




