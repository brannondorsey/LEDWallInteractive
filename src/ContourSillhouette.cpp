//
//  ContourSillhouette.cpp
//  LEDWallVideoPlayer
//
//  Created by bdorse on 1/31/16.
//
//

#include "ContourSillhouette.h"
ContourSillhouette::ContourSillhouette() {
    
}

ContourSillhouette::ContourSillhouette(const ofPath& path, const ofPoint& centroid) {
    _path = path;
    _scaleSum = 1;
    _origCentroid = centroid;
    scaleIncrement = 0.03;
    alphaDecrement = 1.01;
}

ContourSillhouette::~ContourSillhouette() {
    
}

void ContourSillhouette::setColor(const ofColor& color) {
    _color = color;
}

void ContourSillhouette::update() {
    
    _color.a -= alphaDecrement;
    _scaleSum += scaleIncrement;
}

void ContourSillhouette::draw() {
    
    _path.setStrokeWidth(0);
    _path.setFillColor(_color);
    _path.setFilled(true);
    
    ofPath path = _path;
    path.scale(_scaleSum, _scaleSum);
    ofPoint offset = _origCentroid - (_origCentroid * _scaleSum);
    path.draw(offset.x, offset.y);
    //    _path.draw();
    //    ofPoint origCentroid = wall->mainBlob.getCentroid2D();
    //    ofPoint newCentroid = origCentroid * 0.7;
    //    ofPoint offset = origCentroid - newCentroid;
    //
    //    path.scale(0.7, 0.7);
    //    path.setFillColor(ofColor(0, 0, 255));
    //
    //    path.draw(offset.x, offset.y);
    
    //    wall->mainBlob.simplify();
    //    ofSetLineWidth(100);
    //    wall->mainBlob.draw();
}

bool ContourSillhouette::isDead() {
    return _color.a <= 0;
}

