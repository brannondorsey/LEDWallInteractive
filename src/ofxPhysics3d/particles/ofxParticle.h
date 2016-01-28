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

#pragma once
#include "ofMain.h"

class ofxParticle: public ofVec3f{
public:
    ofVec3f oldPos;
    ofVec3f accel;
    ofVec3f origin;
	float drag, mass, invMass, radius;
	bool bActive;
	bool bCollide;
	void init(float _x, float _y, float z, float _radius, float _mass, float _drag);
	

	ofxParticle();
	ofxParticle(float x, float y, float z, float _radius = 10, float _mass = 1, float _drag = 0.8f);
	ofxParticle(ofVec3f _iniPos, float _radius = 10, float _mass = 1, float _drag = 0.8f);
	
	virtual ~ofxParticle(){}
	
	virtual void updateParticle(float timeStep = 1.0f);
	virtual void render(){debugRender();}
	void debugRender();
	
	void applyForce(const ofVec3f& _force);
	void applyImpulse(const ofVec3f& _impulse);
	
	void setDrag(float _drag);
	float getDrag();
	void setMass(float _mass);
	float getMass();
	float getInvMass();
	void setRadius(float _radius);
	float getRadius();
	
	void setActive(bool _bActive);
	bool isActive();
	
	float distanceTo(ofxParticle*p);
	float distanceToSquared(ofxParticle*p);
	
	bool isPointInside(const ofVec3f& p);
	void moveTo(const ofVec3f& _target);
	void moveTo(float _x, float _y, float _z);
	void moveBy(const ofVec3f& _amount);
	void moveBy(float _x, float _y, float _z);
	void lerp(const ofVec3f& _target, float _interp);
	void moveTowards(const ofVec3f& _target, float strength);
	void applyAttractionForce(const ofVec3f& target, float amount);
	void applyRepulsionForce(const ofVec3f& target, float amount); 
	void stopMotion();
	
	void setVel(const ofVec3f& _vel);
	ofVec3f getVel();
	void setSpeed(float _speed);
	
	void enableCollisions(bool _enable);
	bool collisionsEnabled();
	//ofColor color;
	ofVec3f target;
	float targetAttractionForce;
	float repulsionForce;
    
};





