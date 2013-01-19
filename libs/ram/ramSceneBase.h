#pragma once
#include "ofMain.h"
#include "ofxAutoControlPanel.h"

class ramSceneBase
{
public:
	ramSceneBase() : bEnabled(false) {}
	virtual ~ramSceneBase(){}
	
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	virtual void refreshControlPanel(ofxAutoControlPanel& gui) {};
	
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}
	virtual void drawFloor() {}
	
	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { bEnabled ^= true; }
	inline void setEnabled(bool b) { bEnabled = b; }
	
	string getSceneName() { return sceneName.empty() ? "no name" : sceneName; }
	
	ramSceneBase* getPtr() { return this; }
	
protected:
	
	ofxAutoControlPanel *guiPtr;
	
	string sceneName;
	bool bEnabled;
};