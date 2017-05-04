// 
// ramSceneManager.cpp - RAMDanceToolkit
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

#include "ramSceneManager.h"

#include "ramControlPanel.h"
#include "ramPhysics.h"

using namespace rdtk;

extern bool drawModel;

SceneManager* SceneManager::_instance = NULL;

SceneManager& SceneManager::instance()
{
	if (_instance == NULL)
		_instance = new SceneManager;
	return *_instance;
}

SceneManager::SceneManager() {}

SceneManager::SceneManager(const SceneManager&) {}

SceneManager& SceneManager::operator=(const SceneManager&) { return *this; }

void SceneManager::setup()
{
	enableAllEvents();
	
	ofAddListener(ofEvents().update, this, &SceneManager::update);
	ofAddListener(ofEvents().draw, this, &SceneManager::draw);
	ofAddListener(ofEvents().exit, this, &SceneManager::exit);
	
	actorsScene = make_shared<ActorsScene>();
	addScene(actorsScene);
	actorsScene->setEnabled(true);
}

void SceneManager::addScene(ofPtr<BaseScene> scene)
{
	scenes.push_back(scene);
	scene->setup();
	GetGUI().addPanel(ofPtr<Unit>(scene));
	GuiManager::instance().addScene(ofPtr<SceneGui>(scene));
}

size_t SceneManager::getNumScenes() const
{
	return scenes.size();
}
size_t SceneManager::findtSceneIndex(string name) const
{
	for (int i=0; i<scenes.size(); i++)
	{
		string sceneName = scenes.at(i)->getName();
		if (sceneName == name)
			return i;
	}
	return -1;
}
ofPtr<BaseScene> SceneManager::getScene(size_t index)
{
	return scenes.at(index);
}

ofPtr<ActorsScene> SceneManager::getActorsScene()
{
	return actorsScene;
}

void SceneManager::setShowAllActors(bool showAllActors)
{
	actorsScene->showAll(showAllActors);
}

bool SceneManager::getShowAllActors() const
{
	return actorsScene->getShowAll();
}

void SceneManager::update(ofEventArgs& args)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (i >= scenes.size()) break;

		ofPtr<BaseScene> scene = scenes.at(i);
		if (scene->isEnabled())
			scene->update();
		else
			continue;
	}
}

void SceneManager::draw(ofEventArgs& args)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	ofPushStyle();

	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < scenes.size(); i++)
	{
		ofPtr<BaseScene> scene = scenes[i];
		if (!scene->isEnabled()) continue;

		bool enable_physics = GetEnablePhysicsPrimitive();
		EnablePhysicsPrimitive(false);

		if (ShadowEnabled())
		{
			// draw shadow

			BeginShadow();

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			ofPushStyle();

			scene->draw();

			ofPopStyle();
			glPopMatrix();
			glPopAttrib();

			BeginCamera();

			for (int n = 0; n < getNumNodeArray(); n++)
			{
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();

				if (getNodeArray(n).isActor())
				{
					Actor &o = (Actor &)getNodeArray(n);
					scene->drawActor(o);
				}
				else
				{
					RigidBody &o = (RigidBody &)getNodeArray(n);
					scene->drawRigid(o);
				}

				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
			}
			
			map<string, NodeArray>::iterator it = getActorManager().getAllBus().begin();

			while (it != getActorManager().getAllBus().end())
			{
				const NodeArray &o = (*it).second;
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				if (o.isActor())
					scene->drawActor((Actor &)o);
				else
					scene->drawRigid((RigidBody &)o);
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				
				++it;
			}
			
			EndCamera();

			EndShadow();
		}

		EnablePhysicsPrimitive(enable_physics);

		if (drawModel)
		{
			// draw object

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			ofPushStyle();

			scene->draw();

			ofPopStyle();
			glPopMatrix();
			glPopAttrib();

			BeginCamera();

			for (int n = 0; n < getNumNodeArray(); n++)
			{
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();

				if (getNodeArray(n).isActor())
				{
					Actor &o = (Actor &)getNodeArray(n);
					scene->drawActor(o);
				}
				else
				{
					RigidBody &o = (RigidBody &)getNodeArray(n);
					scene->drawRigid(o);
				}

				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
			}
			
			
			map<string, NodeArray>::iterator it = getActorManager().getAllBus().begin();
			
			while (it != getActorManager().getAllBus().end())
			{
				const NodeArray &o = (*it).second;
				
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();
				
				if (o.isActor())
					scene->drawActor((Actor &)o);
				else
					scene->drawRigid((RigidBody &)o);
				
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				++it;
			}

			EndCamera();
		}

		{
			// draw HUD
			ofPushView();
			ofSetupScreen();
			scene->drawHUD();
			ofPopView();
		}
	}

	ofPopStyle();
	glPopMatrix();
	glPopAttrib();
	
	GuiManager::instance().draw();
}

void SceneManager::exit(ofEventArgs& args)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (i >= scenes.size()) break;
        
		ofPtr<BaseScene> scene = scenes.at(i);
        scene->exit();
	}
}

void SceneManager::enableAllEvents()
{
	ofAddListener(ActorManager::instance().actorSetup, this, &SceneManager::actorSetup);
	ofAddListener(ActorManager::instance().actorExit, this, &SceneManager::actorExit);
	ofAddListener(ActorManager::instance().rigidSetup, this, &SceneManager::rigidSetup);
	ofAddListener(ActorManager::instance().rigidExit, this, &SceneManager::rigidExit);
}

void SceneManager::disableAllEvents()
{
	ofRemoveListener(ActorManager::instance().actorSetup, this, &SceneManager::actorSetup);
	ofRemoveListener(ActorManager::instance().actorExit, this, &SceneManager::actorExit);
	ofRemoveListener(ActorManager::instance().rigidSetup, this, &SceneManager::rigidSetup);
	ofRemoveListener(ActorManager::instance().rigidExit, this, &SceneManager::rigidExit);
}

void SceneManager::actorSetup(Actor &actor)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ofPtr<BaseScene> scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onActorSetup(actor);
	}
}

void SceneManager::actorExit(Actor &actor)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ofPtr<BaseScene> scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onActorExit(actor);
	}
}

void SceneManager::rigidSetup(RigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ofPtr<BaseScene> scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onRigidSetup(rigid);
	}
}

void SceneManager::rigidExit(RigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ofPtr<BaseScene> scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onRigidExit(rigid);
	}
}
