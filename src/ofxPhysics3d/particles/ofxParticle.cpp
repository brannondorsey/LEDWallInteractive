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

#include "ofxParticle.h"

ofxParticle::ofxParticle()
{
	init(0, 0, 0, 10, 1, 0.8f);
	target = ofVec3f::zero();
	targetAttractionForce = 0.0f;
	repulsionForce = 0.0f; 
	//color = ofColor::white;
}

ofxParticle::ofxParticle(float _x, float _y, float _z, float _radius, float _mass, float _drag){
	init(_x, _y, _z, _radius, _mass, _drag);
}

ofxParticle::ofxParticle(ofVec3f iniPos, float _radius, float _mass, float _drag){
	init(iniPos.x, iniPos.y, iniPos.z, _radius, _mass, _drag);
}

void ofxParticle::init(float _x, float _y, float _z, float _radius, float _mass, float _drag){
	x = _x;
	y = _y;
	z = _z;
	oldPos.set(x, y, z);
    origin = oldPos;
	mass = _mass;
	invMass = 1.0f/mass;
	radius = _radius;
	drag = _drag;
	bActive = true;
	bCollide = false;
}

void ofxParticle::updateParticle(float timeStep){
	if(!bActive) return;
	float tempX = x, tempY = y, tempZ = z;
	accel *= invMass;
	x += (x - oldPos.x)*drag + accel.x * timeStep*timeStep;
	y += (y - oldPos.y)*drag + accel.y * timeStep*timeStep;
	z += (z - oldPos.z)*drag + accel.z * timeStep*timeStep;
	accel.set(0,0,0);
	oldPos.x = tempX;
	oldPos.y = tempY;
	oldPos.z = tempZ;
}

void ofxParticle::debugRender(){
	ofCircle(x, y, radius);
}

void ofxParticle::applyForce(const ofVec3f& _force){
	accel += _force;	
}

void ofxParticle::applyImpulse(const ofVec3f& _impulse){
	if(bActive){
		x += _impulse.x;
		y += _impulse.y;
		z += _impulse.z;
	}
}

void ofxParticle::setDrag(float _drag){
	drag = _drag;	
}

float ofxParticle::getDrag(){
	return drag;	
}

void ofxParticle::setMass(float _mass){
	mass = _mass;	
	invMass = 1.0f / mass;
}


float ofxParticle::getMass(){
	return mass;	
}

float ofxParticle::getInvMass(){
	return invMass;	
}


void ofxParticle::setRadius(float _radius){
	radius = _radius;	
}

float ofxParticle::getRadius(){
	return radius;	
}

void ofxParticle::setActive(bool _bActive){
	bActive = _bActive;
}

bool ofxParticle::isActive(){
	return bActive;	
}

float ofxParticle::distanceTo(ofxParticle*p){
	ofxParticle& pRef = *p;
	float dx = pRef.x - x;
	float dy = pRef.y - y;
	float dz = pRef.z - z;
	return sqrtf(dx*dx + dy*dy + dz*dz);
}

float ofxParticle::distanceToSquared(ofxParticle*p){
	ofxParticle& pRef = *p;
	float dx = pRef.x - x;
	float dy = pRef.y - y;
	float dz = pRef.z - z;
	return dx*dx + dy*dy + dz*dz;
}

bool ofxParticle::isPointInside(const ofVec3f& p){
	float dx = p.x - x;
	float dy = p.y - y;
	float dz = p.z - z;
	return dx*dx + dy*dy + dz*dz < radius*radius;
}

void ofxParticle::moveTo(const ofVec3f& _target){
	moveTo(_target.x, _target.y, _target.z);
}

void ofxParticle::moveTo(float _x, float _y, float _z){
	float diffX = _x - x;
	float diffY = _y - y;
	float diffZ = _z - z;
	oldPos.x += diffX;
	oldPos.y += diffY;
	oldPos.z += diffZ;
	x = _x;
	y = _y;
	z = _z;
}

void ofxParticle::moveBy(const ofVec3f& _amount){
	moveBy(_amount.x, _amount.y, _amount.z);
}

void ofxParticle::moveBy(float _x, float _y, float _z){
	x += _x;
	y += _y;
	z += _z;
	oldPos.x += _x;
	oldPos.y += _y;
	oldPos.z += _z;
}

void ofxParticle::lerp(const ofVec3f& _target, float _interp){
	ofVec3f diff = _target - *this;
	diff *= _interp;
	moveBy(diff);
}

void ofxParticle::moveTowards(const ofVec3f& _target, float _strength){
    if(!this)
    {
        ofLogError(__func__) << "?????";
        return;
    }
	ofVec3f diff = _target - *this;
	diff *= _strength;
	applyForce(diff);
}

void ofxParticle::applyAttractionForce(const ofVec3f& target, float amount){
	ofVec3f force = target - *this;
	float dist = MIN(1, sqrt(force.x*force.x + force.y*force.y));
	force /= (dist*dist*dist);
	force *= amount;
	applyForce(force);
}

void ofxParticle::applyRepulsionForce(const ofVec3f& target, float amount){
	ofVec3f force = target - *this;
	float dist = MAX(1, sqrt(force.x*force.x + force.y*force.y));
	force /= (dist*dist*dist);
	force *= -amount;
	applyForce(force);
}

void ofxParticle::stopMotion(){
	accel.set(0,0, 0);
	oldPos.set(x, y, z);
}

void ofxParticle::setVel(const ofVec3f& _vel){
	oldPos.x = x - _vel.x;
	oldPos.y = y - _vel.y;
	oldPos.z = z - _vel.z;
}

void ofxParticle::setSpeed(float _speed){
	ofVec3f vel = getVel();
	float velMag = sqrtf(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z);
	vel /= velMag;
	vel *= _speed;
	oldPos.x = x - vel.x;
	oldPos.y = y - vel.y;
	oldPos.z = z - vel.z;
}

ofVec3f ofxParticle::getVel(){
	return ofVec3f(x - oldPos.x, y - oldPos.y);
}

void ofxParticle::enableCollisions(bool _bCollisions){
	bCollide = _bCollisions;
}

bool ofxParticle::collisionsEnabled(){
	return bCollide;
}







