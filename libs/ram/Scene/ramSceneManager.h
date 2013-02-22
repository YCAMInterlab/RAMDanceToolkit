#pragma once

#include "ofMain.h"

#include "ramBaseScene.h"
#include "ramGlobalShortcut.h"

class ramSceneManager : public ramGlobalShortcut
{
public:
	
	void setup(const vector<ramBaseScene*>& scenes);
	
	void update();
	void draw();
	
protected:
	
	void enableAllEvents();
	void disableAllEvents();
	
	void actorSetup(ramActor &actor);
	void actorExit(ramActor &actor);
	
	void rigidSetup(ramRigidBody &rigid);
	void rigidExit(ramRigidBody &rigid);

	vector<ramBaseScene*> scenes;
};

