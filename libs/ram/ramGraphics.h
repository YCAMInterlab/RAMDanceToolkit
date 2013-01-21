#pragma once

#include "ofMain.h"

#include "ramActor.h"

inline void ofLine(const ramNode& o1, const ramNode& o2)
{
	ofLine(o1.getPosition(), o2.getPosition());
}

inline void ofBox(const ramNode& o, float size)
{
	ofBox(o.getPosition(), size);
}

inline void ofSphere(const ramNode& o, float radius)
{
	ofSphere(o.getPosition(), radius);
}

void ramBox(const ramNode& o, float size);
void ramSphere(const ramNode& o, float radius);

void ramBasicFloor(const int floorPattern,
				   const float floorSize = 600.0,
				   const float tileSize = 50.0,
				   const ofColor& c1 = ofColor(60),
				   const ofColor& c2 = ofColor(230));

void ramBasicActor(ramActor& actor, float* matrixPtr = NULL);


