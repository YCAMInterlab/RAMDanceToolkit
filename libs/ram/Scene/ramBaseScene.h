#pragma once

#include "ramUnit.h"
#include "ramControlPanel.h"


class ramBaseScene : public ramUnit
{
public:

	ramBaseScene();
	virtual ~ramBaseScene();
	
	virtual void setupControlPanel();
	
	virtual void setup();
	virtual void update();
	virtual void draw();
	virtual void drawHUD();
	virtual void exit();
	
	virtual void drawActor(const ramActor &actor);
	virtual void drawRigid(const ramRigidBody &rigid);

	// nodeArray events
	virtual void onActorSetup(const ramActor &actor);
	virtual void onActorExit(const ramActor &actor);
	virtual void onRigidSetup(const ramRigidBody &rigid);
	virtual void onRigidExit(const ramRigidBody &rigid);

	ramBaseScene* getPtr();

};