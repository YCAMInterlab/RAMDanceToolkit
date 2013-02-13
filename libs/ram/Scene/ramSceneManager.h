#pragma once

#include "ofMain.h"

#include "ramBaseScene.h"
#include "ramGlobalShortcut.h"

class ramSceneManager : public ramGlobalShortcut
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

