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
		ofAddListener(getActorManager().actorEntered, this, &ramBaseApp::actorEntered );
		ofAddListener(getActorManager().actorExited, this, &ramBaseApp::actorExited );
		ofAddListener(getActorManager().rigidEntered, this, &ramBaseApp::rigidEntered );
		ofAddListener(getActorManager().rigidExited, this, &ramBaseApp::rigidExited );
    }
	void ramDisableAllEvents()
	{
		ofRemoveListener(ofEvents().update, this, &ramBaseApp::update);
		ofRemoveListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofRemoveListener(ofEvents().exit, this, &ramBaseApp::exit);
		ofRemoveListener(getActorManager().actorEntered, this, &ramBaseApp::actorEntered );
		ofRemoveListener(getActorManager().actorExited, this, &ramBaseApp::actorExited );
		ofRemoveListener(getActorManager().rigidEntered, this, &ramBaseApp::rigidEntered );
		ofRemoveListener(getActorManager().rigidExited, this, &ramBaseApp::rigidExited );
	}
    
	virtual void actorEntered(ramActor& actor) {}
	virtual void actorExited(ramActor& actor) {}
	virtual void rigidEntered(ramRigidBody& rigid) {}
	virtual void rigidExited(ramRigidBody& rigid) {}
//	virtual void ramRigidEnter(const ramRigidBody& rigid){
//		cout << rigid.getName() << endl;
//	}
    
	// physics
	virtual void collision(const ramNode& jointA, const ramNode& jointB) {}
    
    
	// Rec & Play....
	void ramRecStart();
	void ramRecStop();
	void ramPlay();
    
<<<<<<< HEAD
    
	
	
=======
>>>>>>> 30dd5af7d72eed1f1037b1b492a64b14afe8b07b
private:
    
	// event callback
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	void exit(ofEventArgs &args);
};
