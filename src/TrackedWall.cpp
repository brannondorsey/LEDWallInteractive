//
//  TrackedWall.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/21/15.
//
//

#include "TrackedWall.h"
#include <algorithm>


TrackedWall::TrackedWall(int id_)
{
    doUpdateBackground = true;
    id = id_;
    videoSource = NULL;
}

void TrackedWall::setup(string videoPath="")
{
    videoSource = new VideoSource();
    if (!videoPath.empty())
    {
        videoSource->setup(videoPath);
    }else
    {
        videoSource->setup(640, 480, 30, id);

    }
    contourFinder.setMinAreaRadius(50);
    contourFinder.setMaxAreaRadius(600);
    contourFinder.setThreshold(20);
    fbo.allocate(videoSource->getWidth(), videoSource->getHeight());
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
}

ofPolyline TrackedWall::getLargestBlob(const vector<ofPolyline>& polylines)
{
    ofPolyline mainBlob;
    float largestArea = 0.0;
    for (int i=0; i<polylines.size(); i++)
    {
        ofPolyline line = polylines[i];
        //ofLogVerbose() << "largestArea: " << largestArea << " line.getArea() " << abs(line.getArea());
        if (abs(line.getArea())>largestArea)
        {
            largestArea = abs(line.getArea());
            mainBlob = line;
        }
    }
    return mainBlob;
}

vector<ofPolyline> TrackedWall::createBlobs(int numBlobs)
{
    blobs.clear();
    float step = .75;
    
    
    for (int i=0; i<numBlobs; i++)
    {
        ofPolyline simple(mainBlob);
        simple.simplify();
        blobs.push_back(simple);
    }
    
    for (int i=0; i<blobs.size(); i++)
    {
        ofVec3f center = blobs[i].getCentroid2D();
        for (int j=0; j<blobs[i].getVertices().size(); j++)
        {
            blobs[i].getVertices()[j]-=center;
            blobs[i].getVertices()[j]*=(float)(i*step);
        }
        blobs[i].close();
    }
    std::reverse(blobs.begin(), blobs.end());
    return blobs;

}


void TrackedWall::drawBlobScaled(int i, ofColor& color)
{
    if(blobs.empty()) return;
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    int alphaStep = 255/blobs.size();
    ofPushMatrix();
    ofPushStyle();
        ofColor wAlpha(color, i*alphaStep);
        ofSetColor(wAlpha);
        ofTranslate(mainBlob.getCentroid2D());
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

void TrackedWall::drawMainBlobPixels(ofColor& color)
{
    if(mainBlob.getVertices().empty()) return;
    
    ofPushStyle();
        int step = 16;
        for (int x = 0; x<fbo.getWidth(); x+=step)
        {
            for (int y = 0; y<fbo.getHeight(); y+=step)
            {
                bool isPointInside = ofPolyline::inside(x, y, mainBlob);
                if(isPointInside)
                {
                    ofSetColor(color);
                    ofRect(x, y, 12, 12);
                }
            }
        }
    ofPopStyle();
}

void TrackedWall::update()
{
    videoSource->update();
    
    if(videoSource->isFrameNew())
    {
        if (doUpdateBackground)
        {
            if(videoSource->isCamera)
            {
                background.update(videoSource->getPixels(),
                                  videoSource->getHeight(),
                                  videoSource->getWidth());
            }else{
                background.update(videoSource->getSource());
            }
            
        }
        cv::subtract(background.backgroundOutputMat, background.frameMat, differenceMat);
        if(!differenceMat.empty())
        {
            ofxCv::blur(differenceMat, 20);
            contourFinder.findContours(differenceMat);
            mainBlob = getLargestBlob(contourFinder.getPolylines());
        }
    }
    if(contourFinder.getPolylines().empty())
    {
        ofPolyline blank;
        mainBlob = blank;
    }
  
}

void TrackedWall::draw()
{
    fbo.begin();
        videoSource->draw(0, 0);
        contourFinder.draw();
        mainBlob.draw();
    fbo.end();
    
    fbo.draw(0, 0);
    if(!differenceMat.empty())
    {
        ofxCv::drawMat(differenceMat, 0, 0);
    }else
    {
        ofLogVerbose() << "EMPTY";
    }

}
