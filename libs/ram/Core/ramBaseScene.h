// 
// ramBaseScene.h - RAMDanceToolkit
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

#include "ramUnit.h"
#include "ramControlPanel.h"
#include "ramBaseHasFbo.h"
#include "ramImGuiPanel.h"

namespace rdtk{
	
	class BaseScene : public BaseHasFbo, public SceneGui
	{
	public:
		
		BaseScene();
		virtual ~BaseScene();
		
		virtual void setupControlPanel();
		
		virtual void setup();
		virtual void update();
		virtual void draw();
		virtual void drawHUD();
		virtual void exit();
		
		virtual void drawActor(const Actor &actor);
		virtual void drawRigid(const RigidBody &rigid);
		
		// nodeArray events
		virtual void onActorSetup(const Actor &actor);
		virtual void onActorExit(const Actor &actor);
		virtual void onRigidSetup(const RigidBody &rigid);
		virtual void onRigidExit(const RigidBody &rigid);
		
	};
}


typedef rdtk::BaseScene RAMDEPRECATED(ramBaseScene);
