#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramActor.h"

#include "ramGlobalShortcut.h"

class ramBaseApp : public ofBaseApp, public ramGlobalShortcut
{
    
public:
	
	ramBaseApp() : draw_floor_auto(true) { ramEnableAllEvents(); };
	virtual ~ramBaseApp() {};
	
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}
	
	void drawFloor();
	void setDrawFloorAuto(bool v = true) { draw_floor_auto = v; }
	
    // events
	void ramEnableAllEvents()
    {
        ofAddListener(ofEvents().update, this, &ramBaseApp::update);
        ofAddListener(ofEvents().draw, this, &ramBaseApp::draw);
        ofAddListener(ofEvents().exit, this, &ramBaseApp::exit);
		
		ofAddListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::onActorSetup);
		ofAddListener(ramActorManager::instance().actorExit, this, &ramBaseApp::onActorExit);
		ofAddListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::onRigidSetup);
		ofAddListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::onRigidExit);
    }
	void ramDisableAllEvents()
	{
		ofRemoveListener(ofEvents().update, this, &ramBaseApp::update);
		ofRemoveListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofRemoveListener(ofEvents().exit, this, &ramBaseApp::exit);
		
		ofRemoveListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::onActorSetup);
		ofRemoveListener(ramActorManager::instance().actorExit, this, &ramBaseApp::onActorExit);
		ofRemoveListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::onRigidSetup);
		ofRemoveListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::onRigidExit);
	}
    
	// nodeArray events
	virtual void actorSetup(ramActor &actor) {}
	virtual void actorExit(ramActor &actor) {}
	
	virtual void rigidSetup(ramRigidBody &rigid) {}
	virtual void rigidExit(ramRigidBody &rigid) {}

	
	// physics event
	virtual void collision(const ramNode& jointA, const ramNode& jointB) {}
	
	void updateWithOscMessage(const ofxOscMessage &m) { getActorManager().updateWithOscMessage(m); }
	
private:
	
	bool draw_floor_auto;
	
	// event callback
	void setup(ofEventArgs &args);
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
	
	void onActorSetup(ramActor &actor) { actorSetup(actor); }
	void onActorExit(ramActor &actor) { actorExit(actor); }
	
	void onRigidSetup(ramRigidBody &rigid) { rigidSetup(rigid); }
	void onRigidExit(ramRigidBody &rigid) { rigidExit(rigid); }

	//
	void drawNodeArrays();
};


