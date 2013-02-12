#pragma once

#include "ofMain.h"

#include "ramSceneBase.h"

class ramSceneManager
{
public:
	
	void setup(vector<ramSceneBase*>& scenes);
	
	void update();
	void draw();
	void drawActor(ramActor &actor);
	void drawRigid(ramRigidBody &rigid);
	
protected:
	vector<ramSceneBase*> scenes;
};

