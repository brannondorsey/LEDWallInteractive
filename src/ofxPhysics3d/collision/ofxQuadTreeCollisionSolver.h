#pragma once

#include "ofxSimpleCollisionsolver.h"
#include "ofxParticle.h"

class ofxQuadTreeCollisionSolver: public ofxCollisionSolverBase, public ofxParticle{
public:
	void solve(vector<ofxParticle*>& particles, int numIterations){
		int numParticles = particles.size();
	}
};
