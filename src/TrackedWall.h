#pragma once

#include "VideoSource.h"
#include "ofxDynamicBackgroundCV.h"

class TrackedWall
{
public:
    
    TrackedWall(int);
    int id;
    void setup(string videoPath);
    void update();
    void draw();
    VideoSource* videoSource;
    ofxDynamicBackgroundCV background;
    cv::Mat differenceMat;
    ofxCv::ContourFinder contourFinder;
    
    bool doUpdateBackground;
    
    ofPolyline mainBlob;
    ofFbo fbo;
    
    ofPolyline getLargestBlob(const vector<ofPolyline>& polylines);
    
    void drawMainBlobPixels(ofColor&);
    void drawBlobScaled(int index, ofColor& color);
    vector<ofPolyline>  createBlobs(int numBlobs);
    
   vector<ofPolyline> blobs;

};