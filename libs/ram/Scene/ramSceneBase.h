#pragma once
#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"
#include "ofxUI.h"


class ramControlPanel;

class ramSceneBase
{
	friend class ramControlPanel;
	
public:
	ramSceneBase() : bEnabled(false), key_enabled("Show"), scene_name("unknown scene") {}
	virtual ~ramSceneBase(){}
	
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	inline ramActor& getActor(string name) { return ramActorManager::instance().getActor(name); }
	inline ramRigidBody& getRigidBody(string name) { return ramActorManager::instance().getRigidBody(name); }
	inline ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }
	
	virtual ofxUICanvas* createScenePanel() { return new ofxUICanvas(0, 0, 320+OFX_UI_GLOBAL_WIDGET_SPACING, ofGetHeight()); }
	
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
	inline bool isEnabled() { return bEnabled; }
	
	inline string getSceneEnableKey() { return key_enabled; }
	inline string getSceneName() { return scene_name.empty() ? "no name" : scene_name; }
	
	ramSceneBase* getPtr() { return this; }
	
protected:
	
	string scene_name, key_enabled;
	
	void setSceneName(string name)
	{
		scene_name = name;
		key_enabled = "Show: " + scene_name;
	}
	
	ramControlPanel& gui() { return *guiPtr; }
	
private:
	
	bool bEnabled;
	ramControlPanel *guiPtr;
	
};
