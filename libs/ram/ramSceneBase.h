#pragma once
#include "ofMain.h"
#include "ramControlPanel.h"

class ramSceneBase
{
public:
	ramSceneBase() : bEnabled(false), key_enabled("Show"), scene_name("unknown scene") {}
	virtual ~ramSceneBase(){}
	
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	inline ramActor& getActor(string name) { return ramActorManager::instance().getActor(name); }
	inline ramRigidBody& getRigidBody(string name) { return ramActorManager::instance().getRigidBody(name); }
	
	virtual void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		
		gui.addPanel(scene_name);
		
		cout << "ramSceneBase::refreshControlPanel() was not overrided at: " +scene_name << endl;
	};
	
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
	
	inline string getSceneEnableKey() { return key_enabled; }
	inline string getSceneName() { return scene_name.empty() ? "no name" : scene_name; }
	
	inline ofMatrix4x4 getMatrix() { return shadow_mat; }
	inline void setMatrix(ofMatrix4x4& m) { shadow_mat = m; }
	
	ramSceneBase* getPtr() { return this; }
	
protected:
	
	ramControlPanel *guiPtr;
	bool bEnabled;
	string scene_name, key_enabled;
	ofMatrix4x4 shadow_mat;
	
	void setSceneName(string name)
	{
		scene_name = name;
		key_enabled = "Show: " + scene_name;
	}
};