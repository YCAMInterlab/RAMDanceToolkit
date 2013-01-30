#pragma once

#include "ofMain.h"

class ramShadow
{
public:
	
	void setup();
	void setLightPosition(ofVec3f pos);
	
	void begin();
	void end();
	
protected:
	
	ofMatrix4x4 shadow_matrix;
	ofShader shader;
	
};
