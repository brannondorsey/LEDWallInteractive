//
//  RowDrawApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "RowDrawApp.h"

void RowDrawApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
}

void RowDrawApp::update()
{
    fbo->begin();
    int boxSize = 10;
    ofPushStyle();
    ofColor color(ofColor::red);
    //ofColor color(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
    
    ofSetColor(color);
    boxSize = 10;
    ofRect(currentX, currentY, boxSize, boxSize);
    if(currentX+boxSize<getWidth())
    {
        currentX+=boxSize;
    }else
    {
        currentX = 0;
        currentY+=boxSize;
    }
    if(currentY>getHeight())
    {
        currentY = 0;
        ofClear(0, 0, 0, 0);
    }
    
    ofPopStyle();
    fbo->end();
}

#if 0
void RowDrawApp::drawRandomBoxes()
{
    ofPushStyle();
    ofColor color(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
    ofSetColor(color);
    int size = ofRandom(10, 40);
    ofRect(ofRandom(player.getWidth()), ofRandom(player.getHeight()), size, size);
    ofPopStyle();
}
#endif







