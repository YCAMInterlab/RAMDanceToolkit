#pragma once

#include "ramPrimitive.h"

#include "ofxBtSoftBodyWorld.h"

class ramSoftBodyPrimitive : public ramBasePrimitive
{
	friend class ramPhysics;

public:

	ramSoftBodyPrimitive();
	~ramSoftBodyPrimitive();

	ofxBt::SoftBody getSoftBody() { return body; }

protected:

	ofxBt::SoftBody body;
	ofxBt::SoftBodyWorld& getWorld();

	void internal_update();
};

class ramRopePrimitive : public ramSoftBodyPrimitive
{
public:

	ramRopePrimitive(const ofVec3f p0, const ofVec3f p1);

protected:

	ofNode controlPoint[2];

};