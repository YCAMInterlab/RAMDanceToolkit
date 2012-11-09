#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramActor.h"
#include "ramActorManager.h"

class ramBaseApp : public ofBaseApp
{
    
public:

	ramBaseApp() {};
	virtual ~ramBaseApp() {};

	virtual void drawFloor() {}
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}

	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	void updateWithOscMessage(const ofxOscMessage &m) { getActorManager().updateWithOscMessage(m); }


	// shortcut to ramActorManager
	size_t getNumActor() { return getActorManager().getNumActor(); }
	ramActor& getActor(int index) { return getActorManager().getActor(index); }
	ramActor& getActor(const string& name) { return getActorManager().getActor(name); }
    
    
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
	virtual void collision(const ramNode &jointA, const ramNode &jointB) {};
    
    
	// Rec & Play....
	void ramRecStart();
	void ramRecStop();
	void ramPlay();
    
    
private:
    
	ofEasyCam cam;

	// event callback
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
};
