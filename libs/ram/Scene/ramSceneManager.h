#pragma once

#include "ofMain.h"

#include "ramBaseScene.h"

class ramSceneManager
{
public:
	
	void setup(vector<ramBaseScene*>& scenes);
	
	void update();
	void draw();
	void drawActor(ramActor &actor);
	void drawRigid(ramRigidBody &rigid);
	
protected:
	vector<ramBaseScene*> scenes;
};

