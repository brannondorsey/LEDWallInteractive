#pragma once

#include "ofxCollisionSolverBase.h"
static bool compare(ofxParticle* a, ofxParticle* b);
bool compare(ofxParticle* a, ofxParticle* b){
	return a->x < b->x;
}

class ofxSortingCollisionSolver: public ofxCollisionSolverBase{
public:
	void solve(vector<ofxParticle*>& particles, int numIterations){
		int numParticles = particles.size();
		if(numParticles < 2)return;
		int maxDist = particles[0]->getRadius();
		int partRadius;
		for(int i=1; i<numParticles; i++){
			partRadius = particles[i]->getRadius();
			if(partRadius > maxDist){
				maxDist = partRadius;
			}
		}
		maxDist *= 2;
		float rest;
		ofPoint delta;
		float dist;
		sort(particles.begin(), particles.end(), compare);
		for(int n=0; n<numIterations; n++){
			for(int i=0; i<numParticles; i++){
				ofxParticle& a = *particles[i];
				for(int j = i-1; j >= 0; j--){
					ofxParticle& b = *particles[j];
					delta.x = a.x - b.x;
					if(fabs(delta.x) > maxDist) break;
					if(!a.isActive() and !b.isActive())continue;
					rest = a.getRadius() + b.getRadius();
					delta.y = a.y - b.y;
					dist = delta.x*delta.x + delta.y*delta.y;
					if(dist > rest*rest)continue;
					dist = dist < 1 ? 1 : sqrt(dist);
					float moveAmount;
					if(a.isActive() and b.isActive()){
						moveAmount = (dist - rest) / (dist * (a.getInvMass() + b.getInvMass()));
						float moveAmountWeighted = moveAmount * a.getInvMass();
						a.x -= delta.x * moveAmountWeighted;
						a.y -= delta.y * moveAmountWeighted;
						moveAmountWeighted = moveAmount * b.getInvMass();
						b.x += delta.x * moveAmountWeighted;
						b.y += delta.y * moveAmountWeighted;
					} else {
						moveAmount = (dist - rest) / dist;
						if(a.isActive()){
							a.x -= delta.x * moveAmount;
							a.y -= delta.y * moveAmount;
						} else {
							b.x += delta.x * moveAmount;
							b.y += delta.y * moveAmount;
						}
					}
				}
			}
		}
	}
};