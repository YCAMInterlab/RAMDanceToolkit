#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramActor.h"
#include "ramActorManager.h"
#include "ramCameraManager.h"

class ramBaseApp : public ofBaseApp
{
    
public:
	
	ramBaseApp() {};
	virtual ~ramBaseApp() {};

	virtual void drawFloor() {}
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}
	
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	inline ramCameraManager& getCameraManager() { return ramCameraManager::instance(); }
	inline ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }
	
	void updateWithOscMessage(const ofxOscMessage &m) { getActorManager().updateWithOscMessage(m); }

	// shortcut to ramActorManager
	size_t getNumActor() { return getActorManager().getNumActor(); }
	ramActor& getActor(int index) { return getActorManager().getActor(index); }
	ramActor& getActor(const string& name) { return getActorManager().getActor(name); }
	
	size_t getNumRigidBody() { return getActorManager().getNumRigidBody(); }
	ramRigidBody& getRigidBody(int index) { return getActorManager().getRigidBody(index); }
	ramRigidBody& getRigidBody(const string& name) { return getActorManager().getRigidBody(name); }
    
    
    // events
	void ramEnableAllEvents()
    {
        ofAddListener(ofEvents().update, this, &ramBaseApp::update);
        ofAddListener(ofEvents().draw, this, &ramBaseApp::draw);
        ofAddListener(ofEvents().exit, this, &ramBaseApp::exit);
    }
	void ramDisableAllEvents()
	{
		ofRemoveListener(ofEvents().update, this, &ramBaseApp::update);
		ofRemoveListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofRemoveListener(ofEvents().exit, this, &ramBaseApp::exit);
	}
    
	// physics
	virtual void collision(const ramNode& jointA, const ramNode& jointB) {}
	
private:
	
	// event callback
	void setup(ofEventArgs &args);
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
};


