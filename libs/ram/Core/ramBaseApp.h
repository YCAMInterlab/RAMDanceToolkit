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

namespace rdtk{
	
	class BaseApp : public ofBaseApp, public GlobalShortcut
	{
		
	public:
		
		BaseApp() : draw_floor_auto(true) { EnableAllEvents(); };
		virtual ~BaseApp() {};
		
		// events
		void EnableAllEvents()
		{
			ofAddListener(ActorManager::instance().actorSetup, this, &BaseApp::actorSetup);
			ofAddListener(ActorManager::instance().actorExit, this, &BaseApp::actorExit);
			ofAddListener(ActorManager::instance().rigidSetup, this, &BaseApp::rigidSetup);
			ofAddListener(ActorManager::instance().rigidExit, this, &BaseApp::rigidExit);
		}
		void DisableAllEvents()
		{
			ofRemoveListener(ActorManager::instance().actorSetup, this, &BaseApp::actorSetup);
			ofRemoveListener(ActorManager::instance().actorExit, this, &BaseApp::actorExit);
			ofRemoveListener(ActorManager::instance().rigidSetup, this, &BaseApp::rigidSetup);
			ofRemoveListener(ActorManager::instance().rigidExit, this, &BaseApp::rigidExit);
		}
		
		
		virtual void drawActor(const Actor &actor) {}
		virtual void drawRigid(const RigidBody &rigid) {}
		
		// nodeArray events
		virtual void onActorSetup(const Actor &actor) {}
		virtual void onActorExit(const Actor &actor) {}
		
		virtual void onRigidSetup(const RigidBody &rigid) {}
		virtual void onRigidExit(const RigidBody &rigid) {}
		
		// physics event
		virtual void collision(const Node& jointA, const Node& jointB) {}
		
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
		
		void actorSetup(Actor &actor) { onActorSetup(actor); }
		void actorExit(Actor &actor) { onActorExit(actor); }
		
		void rigidSetup(RigidBody &rigid) { onRigidSetup(rigid); }
		void rigidExit(RigidBody &rigid) { onRigidExit(rigid); }
		
		//
		void drawNodeArrays();
	};
}

typedef rdtk::BaseApp RAMDEPRECATED(ramBaseApp);
