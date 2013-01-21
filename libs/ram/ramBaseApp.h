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
    
	
	enum FloorPattern
	{
		FLOOR_PLANE				= 0,
		FLOOR_CHECKER_PATTERN	= 1,
		FLOOR_GRID_LINES		= 2,
		FLOOR_NONE				= 3
	};
	
	enum ColorId
	{
		COLOR_RED_NORMAL	= 0,
		COLOR_RED_DEEP		= 1,
		COLOR_RED_LIGHT		= 2,
		
		COLOR_GREEN_NORMAL	= 3,
		COLOR_GREEN_DEEP	= 4,
		COLOR_GREEN_LIGHT	= 5,
		
		COLOR_BLUE_NORMAL	= 6,
		COLOR_BLUE_DEEP		= 7,
		COLOR_BLUE_LIGHT	= 8,
		
		COLOR_YELLOW_NORMAL = 9,
		COLOR_YELLOW_DEEP	= 10,
		COLOR_YELLOW_LIGHT  = 11,
		
		COLOR_BLACK	= 12,
		COLOR_GRAY	= 13,
		COLOR_WHITE	= 14,
		
		NUM_COLORS = 15
	};
	
private:
	
	// event callback
	void setup(ofEventArgs &args);
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
};
