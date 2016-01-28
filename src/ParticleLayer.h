#pragma once
#include "ofxPhysics3d.h"


class ParticleLayer
{
public:
    ParticleLayer();
    void setup(int particleSize_);
    void update();
    void updateFBO(ofTexture* texture);
    float massLowerLimit;
    float massUpperLimit;
    vector<ofxParticle*>particles;
    
    ofxPhysics3d *physics;
    ofxParticle* addParticle(ofPoint point);
    
    void movePointsTowards(vector<ofVec3f>& points,  int xOffset);
    void movePointsTowardsOrigin();
    
    int particleSize;
};


