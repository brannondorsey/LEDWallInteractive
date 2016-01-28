/* 
 * Copyright (c) 2009, Rui Madeira
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * http://creativecommons.org/licenses/LGPL/2.1/
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ofxPhysics3d.h"


ofxPhysics3d::ofxPhysics3d(ofVec3f _grav, bool _bCollisions, int _numIterations, ofVec3f _worldMin, ofVec3f _worldMax, bool _bCheckBounds, bool _bGravity, ofxCollisionSolverBase* _collisionSolver){
	gravity.set(_grav.x, _grav.y);
	bCollisions = _bCollisions;
	numIterations = _numIterations;
	bCheckBounds = _bCheckBounds;
	bGravity = _bGravity;
	worldMin.set(_worldMin.x, _worldMin.y);
	worldMax.set(_worldMax.x, _worldMax.y);
	collisionSolver = NULL;
	if(_collisionSolver == NULL){
		//setCollisionSolver(new ofxSimpleCollisionSolver());
		setCollisionSolver(new ofxSortingCollisionSolver());
	} else {
		setCollisionSolver(_collisionSolver);
	}
	numConstraints = 0;
}

ofxPhysics3d::~ofxPhysics3d(){
	for(int i=0; i<particles.size(); i++){
		delete particles[i];	
	}
	particles.clear();
	for(int i=0; i<constraints.size(); i++){
		delete constraints[i];
	}
	constraints.clear();
	delete collisionSolver;
}

void ofxPhysics3d::setCollisionSolver(ofxCollisionSolverBase* cs){
	if(collisionSolver){
		delete collisionSolver;
	}
	collisionSolver = cs;
}

void ofxPhysics3d::update(float timeStep){
	numConstraints = constraints.size();
	if(bGravity){
		for(int i=0; i<particles.size(); i++){
			particles[i]->applyImpulse(gravity);	
		}
	}
	for(int i=0; i<particles.size(); i++){
		particles[i]->updateParticle(timeStep);
	}
	for(int n=0; n<numIterations; n++){
		for(int i=0; i<numConstraints; i++){
			constraints[i]->update();
		}
		if(bCheckBounds){
			boundsConstrain();
		}
	}
	if(bCollisions) collisionSolver->solve(particles, numIterations);
}

void ofxPhysics3d::renderParticles(){
	for(int i=0; i<particles.size(); i++){
		particles[i]->render();
	}
}

void ofxPhysics3d::renderConstraints(){
	for(int i=0; i<numConstraints; i++){
		constraints[i]->render();	
	}
}

void ofxPhysics3d::debugRender(){
	renderParticles();
	renderConstraints();
}

void ofxPhysics3d::addParticle(ofxParticle* _p, bool _enableCollisions){
	particles.push_back(_p);
	_p->enableCollisions(_enableCollisions);
}

void ofxPhysics3d::deleteParticle(ofxParticle* _p){
	for(int i=0; i<particles.size(); i++){
		if(_p == particles[i]){
			particles.erase(particles.begin() + i);
			delete _p;
			return;
		}
	}
}

void ofxPhysics3d::removeParticle(ofxParticle*p){
	for(int i=0; i<particles.size(); i++){
		if(particles[i] == p){
			particles.erase(particles.begin() + i);
			return;
		}
	}
}

void ofxPhysics3d::addConstraint(ofxConstraint* _c){
	constraints.push_back(_c);
}

void ofxPhysics3d::deleteConstraint(ofxConstraint*_c){
	for(int i=0; i<constraints.size(); i++){
		if(_c == constraints[i]){
			delete constraints[i];
			constraints.erase(constraints.begin() + i);
			return;
		}
	}
}

void ofxPhysics3d::add(ofxParticle*p){
	particles.push_back(p);
	p->enableCollisions(bCollisions);
}

void ofxPhysics3d::add(ofxParticle*p, bool _enableCollisions){
	particles.push_back(p);
	p->enableCollisions(_enableCollisions);
}

void ofxPhysics3d::add(ofxConstraint*c){
	constraints.push_back(c);
}

bool ofxPhysics3d::hasParticle(ofxParticle*p){
	for(int i=0; i<particles.size(); i++){
		if(particles[i] == p) return true;	
	}
	return false;
}

bool ofxPhysics3d::hasConstraint(ofxConstraint*c){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i] == c) return true;	
	}
	return false;
}

bool ofxPhysics3d::has(ofxParticle*p){
	return hasParticle(p);
}

bool ofxPhysics3d::has(ofxConstraint*c){
	return hasConstraint(c);	
}

void ofxPhysics3d::setNumIterations(unsigned int _n){
	numIterations = _n;	
}

int ofxPhysics3d::getNumIterations(){
	return numIterations;	
}

void ofxPhysics3d::enableCollisions(bool _bEnable){
	bCollisions = _bEnable;
}

bool ofxPhysics3d::collisionsEnabled(){
	return bCollisions;	
}

void ofxPhysics3d::checkBounds(bool _bCheck){
	bCheckBounds = _bCheck;
}

bool ofxPhysics3d::boundsCheck(){
	return bCheckBounds;
}

void ofxPhysics3d::enableGravity(bool _bGravity){
	bGravity = _bGravity;
}

bool ofxPhysics3d::gravityEnabled(){
	return bGravity;
}

void ofxPhysics3d::removeConstraintsWithParticle(ofxParticle*p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			constraints.erase(constraints.begin() + i);
		}
	}
}

bool ofxPhysics3d::hasConstraintsWithParticle(ofxParticle*p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p)return true;
	}
	return false;
}

void ofxPhysics3d::deleteConstraintsWithParticle(ofxParticle *p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			delete constraints[i];
			constraints.erase(constraints.begin() + i);
		}
	}
}

ofxConstraint* ofxPhysics3d::getConstraintWithParticle(ofxParticle *p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			return constraints[i];	
		}
	}
	return NULL;
}

ofxParticle* ofxPhysics3d::getNearestParticle(ofVec3f point){
	if(particles.size() == 0) return NULL;
	if(particles.size() == 1) return particles[0];
	int _index = 0;
	float minDistSQ, dx, dy, distSQ;
	dx = particles[0]->x - point.x;
	dy = particles[0]->y - point.y;
	minDistSQ = dx*dx+dy*dy;
	for(int i=1; i<particles.size(); i++){
		dx = particles[i]->x - point.x;
		dy = particles[i]->y - point.y;
		distSQ = dx*dx+dy*dy;
		if(distSQ < minDistSQ){
			minDistSQ = distSQ;
			_index = i;
		}
	}
	return particles[_index];
}

ofxParticle* ofxPhysics3d::getParticleUnderPoint(ofVec3f& point){
	for(int i=0; i<particles.size(); i++){
		ofxParticle& p = *particles[i];
		float dx = p.x - point.x;
		float dy = p.y - point.y;
		float dz = p.z - point.z;
		float radius = p.getRadius();
		if(dx*dx+dy*dy+dz*dz < radius*radius) return particles[i];
	}
	return NULL;
}


void ofxPhysics3d::boundsConstrain(){
	float pRadius;
	for(int i=0; i<particles.size(); i++){
		ofxParticle& pRef = *particles[i];
		pRadius = pRef.getRadius();
		pRef.x = MAX(worldMin.x + pRadius, MIN(worldMax.x - pRadius, pRef.x));
		pRef.y = MAX(worldMin.y + pRadius, MIN(worldMax.y - pRadius, pRef.y));
		pRef.z = MAX(worldMin.z + pRadius, MIN(worldMax.z - pRadius, pRef.z));
	}
}

int ofxPhysics3d::getNumParticles(){
	return particles.size();	
}

int ofxPhysics3d::getNumConstraints(){
	return numConstraints;	
}

vector<ofxParticle*>& ofxPhysics3d::getParticles(){
	return particles;
}

vector<ofxConstraint*>& ofxPhysics3d::getConstraints(){
	return constraints;
}

ofVec3f& ofxPhysics3d::getGravity(){
	return gravity;
}

void ofxPhysics3d::setGravity(ofVec3f _gravity){
	gravity.set(_gravity.x, _gravity.y);
}

void ofxPhysics3d::removeAllParticles(){
	particles.clear();
}

void ofxPhysics3d::deleteAllParticles(){
	for(int i=0; i<particles.size(); i++){
		delete particles[i];
	}
	particles.clear();
}

void ofxPhysics3d::removeAllConstraints(){
	constraints.clear();
}

void ofxPhysics3d::deleteAllConstraints(){
	for(int i=0; i<numConstraints; i++){
		delete constraints[i];
	}
	constraints.clear();
}

void ofxPhysics3d::removeAll(){
	removeAllParticles();
	removeAllConstraints();
}

void ofxPhysics3d::deleteAll(){
	deleteAllParticles();
	deleteAllConstraints();
}

void ofxPhysics3d::reserveForParticles(int numberOfParticles){
	particles.reserve(numberOfParticles);
}

void ofxPhysics3d::reserveForConstraints(int numberOfConstraints){
	constraints.reserve(numberOfConstraints);
}











