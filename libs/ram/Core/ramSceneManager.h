// 
// ramSceneManager.h - RAMDanceToolkit
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

#include "ramBaseScene.h"
#include "ramGlobal.h"
#include "ramActorsScene.h"

namespace rdtk{
	class SceneManager : public GlobalShortcut
	{
	public:
		
		static SceneManager& instance();
		
		void setup();
		void addScene(BaseScene* scene);
		
		size_t getNumScenes() const;
		size_t findtSceneIndex(string name) const;
		BaseScene* getScene(size_t index) const;
		
		ActorsScene* getActorsScene();
		void setShowAllActors(bool showAllActors);
		bool getShowAllActors() const;
		
	protected:
		
		void enableAllEvents();
		void disableAllEvents();
		
		void actorSetup(Actor &actor);
		void actorExit(Actor &actor);
		
		void rigidSetup(RigidBody &rigid);
		void rigidExit(RigidBody &rigid);
		
		vector<BaseScene*> scenes;
		
		void update(ofEventArgs& args);
		void draw(ofEventArgs& args);
		void exit(ofEventArgs& args);
		
	private:
		
		static SceneManager *_instance;
		SceneManager();
		SceneManager(const SceneManager&);
		SceneManager& operator=(const SceneManager&);
		
		ActorsScene* actorsScene;
	};
}

typedef rdtk::SceneManager OF_DEPRECATED(ramSceneManager);
