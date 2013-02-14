#pragma once

#include "ofMain.h"

#include "ramGraphics.h"

class ramSimpleShadow
{
public:
	
	void setup();
	void setLightPosition(ofVec3f pos);
	void setShadowColor(ofFloatColor color);
	
	void begin();
	void end();
	
	void setShadowAlpha(float alpha);
	
protected:
	
	ofMatrix4x4 shadow_matrix;
	ofShader shader;
	
	ofFloatColor shadow_color;
	
};
