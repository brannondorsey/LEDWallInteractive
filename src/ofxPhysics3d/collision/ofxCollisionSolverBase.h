#pragma once

#include <vector>
#include "ofxParticle.h"

class ofxCollisionSolverBase{
public:	
	virtual void solve(vector<ofxParticle*>& particles, int numIterations) = 0;
};

