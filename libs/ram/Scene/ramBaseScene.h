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
	
	virtual void drawActor(const ramActor &actor);
	virtual void drawRigid(const ramRigidBody &rigid);

	inline void enable();
	inline void disable();
	inline void toggle();
	inline bool isEnabled();
	inline void setEnabled(bool enabled);
	
	virtual void onEnabled();
	virtual void onDisabled();

	// nodeArray events
	virtual void onActorSetup(const ramActor &actor);
	virtual void onActorExit(const ramActor &actor);
	virtual void onRigidSetup(const ramRigidBody &rigid);
	virtual void onRigidExit(const ramRigidBody &rigid);

	ramBaseScene* getPtr();
	
private:

	bool bEnabled;

};