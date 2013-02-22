#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramActor.h"

#include "ramGlobal.h"

class ramBaseApp : public ofBaseApp, public ramGlobalShortcut
{
    
public:
	
	ramBaseApp() : draw_floor_auto(true) { ramEnableAllEvents(); };
	virtual ~ramBaseApp() {};
	
	virtual void drawActor(const ramActor &actor) {}
	virtual void drawRigid(const ramRigidBody &rigid) {}
	
	void drawFloor();
	void setDrawFloorAuto(bool v = true) { draw_floor_auto = v; }
	
    // events
	void ramEnableAllEvents()
    {
        ofAddListener(ofEvents().update, this, &ramBaseApp::update);
        ofAddListener(ofEvents().draw, this, &ramBaseApp::draw);
        ofAddListener(ofEvents().exit, this, &ramBaseApp::exit);
		
		ofAddListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::actorSetup);
		ofAddListener(ramActorManager::instance().actorExit, this, &ramBaseApp::actorExit);
		ofAddListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::rigidSetup);
		ofAddListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::rigidExit);
    }
	void ramDisableAllEvents()
	{
		ofRemoveListener(ofEvents().update, this, &ramBaseApp::update);
		ofRemoveListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofRemoveListener(ofEvents().exit, this, &ramBaseApp::exit);
		
		ofRemoveListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::actorSetup);
		ofRemoveListener(ramActorManager::instance().actorExit, this, &ramBaseApp::actorExit);
		ofRemoveListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::rigidSetup);
		ofRemoveListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::rigidExit);
	}
    
	// nodeArray events
	virtual void onActorSetup(const ramActor &actor) {}
	virtual void onActorExit(const ramActor &actor) {}
	
	virtual void onRigidSetup(const ramRigidBody &rigid) {}
	virtual void onRigidExit(const ramRigidBody &rigid) {}
	
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
	
	void actorSetup(ramActor &actor) { onActorSetup(actor); }
	void actorExit(ramActor &actor) { onActorExit(actor); }
	
	void rigidSetup(ramRigidBody &rigid) { onRigidSetup(rigid); }
	void rigidExit(ramRigidBody &rigid) { onRigidExit(rigid); }

	//
	void drawNodeArrays();
};


