#pragma once

#include "ofMain.h"

#include "ramSceneBase.h"

class ramSceneManager
{
public:
	
	void setup(vector<ramSceneBase*>& scenes);
	
	void update();
	void draw();
	
protected:
	
	vector<ramSceneBase*> scenes;
	
};

