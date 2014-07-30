// 
// ramBaseApp.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

	// events
	void ramEnableAllEvents()
	{
		ofAddListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::actorSetup);
		ofAddListener(ramActorManager::instance().actorExit, this, &ramBaseApp::actorExit);
		ofAddListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::rigidSetup);
		ofAddListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::rigidExit);
	}
	void ramDisableAllEvents()
	{
		ofRemoveListener(ramActorManager::instance().actorSetup, this, &ramBaseApp::actorSetup);
		ofRemoveListener(ramActorManager::instance().actorExit, this, &ramBaseApp::actorExit);
		ofRemoveListener(ramActorManager::instance().rigidSetup, this, &ramBaseApp::rigidSetup);
		ofRemoveListener(ramActorManager::instance().rigidExit, this, &ramBaseApp::rigidExit);
	}
	
	
	virtual void drawActor(const ramActor &actor) {}
	virtual void drawRigid(const ramRigidBody &rigid) {}

	// nodeArray events
	virtual void onActorSetup(const ramActor &actor) {}
	virtual void onActorExit(const ramActor &actor) {}

	virtual void onRigidSetup(const ramRigidBody &rigid) {}
	virtual void onRigidExit(const ramRigidBody &rigid) {}

	// physics event
	virtual void collision(const ramNode& jointA, const ramNode& jointB) {}
	
	void drawFloor();
	void setDrawFloorAuto(bool v = true) { draw_floor_auto = v; }
	void updateWithOscMessage(const ofxOscMessage &m) { getActorManager().updateWithOscMessage(m); }

protected:
	virtual void update() {}
	virtual void draw() {}
	virtual void exit() {}
	
private:

	bool draw_floor_auto;
	
	// event callback
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