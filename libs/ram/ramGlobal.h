#pragma once

// TODO: add setting panel, shadow matrix

#include "ofMain.h"

#include "ramControlPanel.h"

class ramGlobalContext
{
public:
	
	void init();

	void setLightPosition(ofVec3f pos);
	
	void beginShadowMatrix();
	void endShadowMatrix();

private:
	
	ofMatrix4x4 shadow_matrix;
};

ramGlobalContext& ramGlobal();
string ramToResourcePath(string path);