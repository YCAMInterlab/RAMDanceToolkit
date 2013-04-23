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

extern bool drawModel;

ramSceneManager* ramSceneManager::_instance = NULL;

ramSceneManager& ramSceneManager::instance()
{
	if (_instance == NULL)
		_instance = new ramSceneManager;
	return *_instance;
}

ramSceneManager::ramSceneManager() {}

ramSceneManager::ramSceneManager(const ramSceneManager&) {}

ramSceneManager& ramSceneManager::operator=(const ramSceneManager&) { return *this; }

void ramSceneManager::setup()
{
	enableAllEvents();
	
	ofAddListener(ofEvents().update, this, &ramSceneManager::update);
	ofAddListener(ofEvents().draw, this, &ramSceneManager::draw);
	ofAddListener(ofEvents().exit, this, &ramSceneManager::exit);
	
	// memory leak on exit
	actorsScene = new ramActorsScene();
	scenes.push_back(actorsScene);
	actorsScene->setup();
	actorsScene->setEnabled(true);
	ramGetGUI().addPanel(actorsScene, false);
}

void ramSceneManager::addScene(ramBaseScene* scene)
{
	scenes.push_back(scene);
	scene->setup();
	ramGetGUI().addPanel(scene);
}

size_t ramSceneManager::getNumScenes() const
{
	return scenes.size();
}
size_t ramSceneManager::findtSceneIndex(string name) const
{
	for (int i=0; i<scenes.size(); i++)
	{
		string sceneName = scenes.at(i)->getName();
		if (sceneName == name)
			return i;
	}
	return -1;
}
ramBaseScene* ramSceneManager::getScene(size_t index) const
{
	return scenes.at(index);
}

ramActorsScene* ramSceneManager::getActorsScene()
{
	return actorsScene;
}

void ramSceneManager::setShowAllActors(bool showAllActors)
{
	return actorsScene->showAll(showAllActors);
}

bool ramSceneManager::getShowAllActors() const
{
	return actorsScene->getShowAll();
}

void ramSceneManager::update(ofEventArgs& args)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (i >= scenes.size()) break;

		ramBaseScene *scene = scenes.at(i);
		if (scene->isEnabled())
			scene->update();
		else
			continue;
	}
}

void ramSceneManager::draw(ofEventArgs& args)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	ofPushStyle();

	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;

		bool enable_physics = ramGetEnablePhysicsPrimitive();
		ramEnablePhysicsPrimitive(false);

		if (ramShadowEnabled())
		{
			// draw shadow

			ramBeginShadow();

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			ofPushStyle();

			scene->draw();

			ofPopStyle();
			glPopMatrix();
			glPopAttrib();

			ramBeginCamera();

			for (int n = 0; n < getNumNodeArray(); n++)
			{
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();

				if (getNodeArray(n).isActor())
				{
					ramActor &o = (ramActor &)getNodeArray(n);
					scene->drawActor(o);
				}
				else
				{
					ramRigidBody &o = (ramRigidBody &)getNodeArray(n);
					scene->drawRigid(o);
				}

				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
			}
			
			map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();

			while (it != getActorManager().getAllBus().end())
			{
				const ramNodeArray &o = (*it).second;
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				if (o.isActor())
					scene->drawActor((ramActor &)o);
				else
					scene->drawRigid((ramRigidBody &)o);
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				
				++it;
			}
			
			ramEndCamera();

			ramEndShadow();
		}

		ramEnablePhysicsPrimitive(enable_physics);

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

			ramBeginCamera();

			for (int n = 0; n < getNumNodeArray(); n++)
			{
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();

				if (getNodeArray(n).isActor())
				{
					ramActor &o = (ramActor &)getNodeArray(n);
					scene->drawActor(o);
				}
				else
				{
					ramRigidBody &o = (ramRigidBody &)getNodeArray(n);
					scene->drawRigid(o);
				}

				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
			}
			
			
			map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();
			
			while (it != getActorManager().getAllBus().end())
			{
				const ramNodeArray &o = (*it).second;
				
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				ofPushStyle();
				
				if (o.isActor())
					scene->drawActor((ramActor &)o);
				else
					scene->drawRigid((ramRigidBody &)o);
				
				
				ofPopStyle();
				glPopMatrix();
				glPopAttrib();
				++it;
			}

			ramEndCamera();
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
}

void ramSceneManager::exit(ofEventArgs& args)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (i >= scenes.size()) break;
        
		ramBaseScene *scene = scenes.at(i);
        scene->exit();
	}
}

void ramSceneManager::enableAllEvents()
{
	ofAddListener(ramActorManager::instance().actorSetup, this, &ramSceneManager::actorSetup);
	ofAddListener(ramActorManager::instance().actorExit, this, &ramSceneManager::actorExit);
	ofAddListener(ramActorManager::instance().rigidSetup, this, &ramSceneManager::rigidSetup);
	ofAddListener(ramActorManager::instance().rigidExit, this, &ramSceneManager::rigidExit);
}

void ramSceneManager::disableAllEvents()
{
	ofRemoveListener(ramActorManager::instance().actorSetup, this, &ramSceneManager::actorSetup);
	ofRemoveListener(ramActorManager::instance().actorExit, this, &ramSceneManager::actorExit);
	ofRemoveListener(ramActorManager::instance().rigidSetup, this, &ramSceneManager::rigidSetup);
	ofRemoveListener(ramActorManager::instance().rigidExit, this, &ramSceneManager::rigidExit);
}

void ramSceneManager::actorSetup(ramActor &actor)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onActorSetup(actor);
	}
}

void ramSceneManager::actorExit(ramActor &actor)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onActorExit(actor);
	}
}

void ramSceneManager::rigidSetup(ramRigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onRigidSetup(rigid);
	}
}

void ramSceneManager::rigidExit(ramRigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->onRigidExit(rigid);
	}
}