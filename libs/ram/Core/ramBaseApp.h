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
	
	void setDrawFloorAuto(bool v = true) { v = draw_floor_auto; }
	
	void updateWithOscMessage(const ofxOscMessage &m) { getActorManager().updateWithOscMessage(m); }

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
	
	bool draw_floor_auto;
	
	// event callback
	void setup(ofEventArgs &args);
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
	
	//
	void drawNodeArrays();
};


