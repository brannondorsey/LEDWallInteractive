//
//  ParticleLayer.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/22/15.
//
//

#include "ParticleLayer.h"


ParticleLayer::ParticleLayer()
{
    physics = NULL;
}

void ParticleLayer::setup(int particleSize_)
{
    particleSize = particleSize_;
    massLowerLimit = 0;
    massUpperLimit = 400;
    ofVec3f gravity(0.0, 0.0, 0.0);
    
    bool doCollisions = false;
    int numIterations = 2;
    ofVec3f worldMin(-ofGetWidth()*2, -ofGetHeight()*2, -1000);
    ofVec3f worldMax (ofGetWidth()*2, ofGetHeight()*2, 1000);
    bool doCheckBounds = false;
    bool doGravity = false;
    
    physics = new ofxPhysics3d(gravity,
                               doCollisions,
                               numIterations,
                               worldMin,
                               worldMax,
                               doCheckBounds,
                               doGravity,
                               NULL);


    
    
}

void ParticleLayer::update()
{
    physics->update();
}



ofxParticle* ParticleLayer::addParticle(ofVec3f point)
{
    float mass = ofRandom(massLowerLimit, massUpperLimit);
    float radius = particleSize;
    float drag = 0.8f;
    ofxParticle *particle = new ofxParticle(point, radius, mass, drag);
    //particle->color = ofColor::red;
    physics->addParticle(particle, false);
    particles = physics->getParticles();
    return particle;
}


void ParticleLayer::movePointsTowards(vector<ofVec3f>& points, int xOffset)
{
    for (int i=0; i<particles.size(); i++)
    {
        int randomIndex = ofRandom(points.size());
        ofVec3f target(points[randomIndex].x+xOffset, points[randomIndex].y, 0);
        
        particles[i]->moveTowards(target, 5.0f);
    }

}

void ParticleLayer::movePointsTowardsOrigin()
{
    for (int i=0; i<particles.size(); i++)
    {
        particles[i]->moveTowards(particles[i]->origin, 0.25f * (float)i);
        
    }
}


