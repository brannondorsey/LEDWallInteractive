#pragma once

#include "ofxCollisionSolverBase.h"

class ofxSimpleCollisionSolver:public ofxCollisionSolverBase{
	void solve(vector<ofxParticle*>& particles, int numIterations){
		int numParticles = particles.size();
		float rest;
		ofPoint delta;
		float dist;
		for(int n=0; n<numIterations; n++){
			for(int i=0; i<numParticles; i++){
				ofxParticle& a = *particles[i];
				for(int j=0; j<i; j++){
					ofxParticle& b = *particles[j];
					if(!a.isActive() and !b.isActive())continue;
					rest = a.getRadius() + b.getRadius();
					delta.x = a.x - b.x;
					delta.y = a.y - b.y;
					delta.z = a.z - b.z;
					dist = delta.x*delta.x + delta.y*delta.y + delta.z*delta.z;
					if(dist > rest*rest)continue;
					dist = dist < 1 ? 1 : sqrt(dist);
					float moveAmount;
					if(a.isActive() and b.isActive()){
						moveAmount = (dist - rest) / (dist * (a.getInvMass() + b.getInvMass()));
						float moveAmountWeighted = moveAmount * a.getInvMass();
						a.x -= delta.x * moveAmountWeighted;
						a.y -= delta.y * moveAmountWeighted;
						a.z -= delta.z * moveAmountWeighted;
						moveAmountWeighted = moveAmount * b.getInvMass();
						b.x += delta.x * moveAmountWeighted;
						b.y += delta.y * moveAmountWeighted;
						b.z += delta.z * moveAmountWeighted;
					} else{
						moveAmount = (dist - rest) / dist;
						if(a.isActive()){
							a.x -= delta.x * moveAmount;
							a.y -= delta.y * moveAmount;
							a.z -= delta.z * moveAmount;
						} else {
							b.x += delta.x * moveAmount;
							b.y += delta.y * moveAmount;
							b.z += delta.z * moveAmount;
						}
					}
				}
			}
		}
	}
};