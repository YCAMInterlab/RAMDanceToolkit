#pragma once

#include "ofMain.h"
#include "ramPhysics.h"

class ramPrimitive : public ofNode
{
	friend class ramPhysics;
	
public:
	
	ramPrimitive();
	~ramPrimitive();
	
	// enablePhysics
	
protected:
	
	ofxBt::Rigid rigid;
	ofxBt::World& getWorld();
	
	void _update();
};

class ramBoxPrimitive : public ramPrimitive
{
public:
	ramBoxPrimitive(const ofVec3f& pos, float size = 100);
	ramBoxPrimitive(const ofVec3f& pos, const ofVec3f& size);
	
	ramBoxPrimitive(const ofMatrix4x4& mat, float size = 100);
	ramBoxPrimitive(const ofMatrix4x4& mat, const ofVec3f& size);

protected:
	void customDraw();
};

class ramSpherePrimitive : public ramPrimitive
{
public:
	ramSpherePrimitive(const ofVec3f& pos, float size = 100);
	
protected:
	void customDraw();
};

