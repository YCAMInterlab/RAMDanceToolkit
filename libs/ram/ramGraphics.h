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

void ramBasicFloor(const float floorSize, const float tileSize, ofColor c1 = ofColor(200), ofColor c2 = ofColor(230));
void ramBasicActor(ramActor& actor);
