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
	
	void _update();
};

class ramCube : public ramPrimitive
{
public:
	
	ramCube(float size = 100);
	
	void customDraw()
	{
		glScalef(size.x, size.y, size.z);
		ofBox(1);
	}
	
private:
	
	ofVec3f size;
};