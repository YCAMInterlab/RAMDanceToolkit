#include "ramSceneManager.h"

#include "ramSharedData.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

void ramSceneManager::setup(vector<ramSceneBase*>& scenes_)
{
	scenes = scenes_;
	
	ramGetGUI().setupSceneToggles(scenes);
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramSceneBase *scene = scenes.at(i);
		scene->setup();
		ramGetGUI().addPanel(scene);
	}
}

void ramSceneManager::update()
{
	for (int i=0; i<scenes.size(); i++)
	{
		if (i >= scenes.size()) break;
		
		ramSceneBase *scene = scenes.at(i);
		if (scene->isEnabled())
			scene->update();
		else
			continue;
	}
}

void ramSceneManager::draw()
{
	ramActorManager &AM = ramActorManager::instance();
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramSceneBase *scene = scenes[i];
		if (!scene->isEnabled()) continue;

		bool enable_physics = ramGetEnablePhysicsPrimitive();
		ramSetEnablePhysicsPrimitive(false);
		
		{
			// draw shadow
			
			ramSharedData::instance().shadow.begin();
			
			scene->draw();
			
			ramBeginCamera();
			
			for (int n = 0; n < AM.getNumActor(); n++)
			{
				ramActor &o = AM.getActor(n);
				scene->drawActor(o);
			}
			
			for (int n = 0; n < AM.getNumRigidBody(); n++)
			{
				ramRigidBody &o = AM.getRigidBody(n);
				scene->drawRigid(o);
			}
			
			ramEndCamera();
			
			ramSharedData::instance().shadow.end();
		}

		ramSetEnablePhysicsPrimitive(enable_physics);
		
		{
			// draw object
			
			scene->draw();
			
			ramBeginCamera();
			
			for (int n = 0; n < AM.getNumActor(); n++)
			{
				ramActor &o = AM.getActor(n);
				scene->drawActor(o);
			}
			
			for (int n = 0; n < AM.getNumRigidBody(); n++)
			{
				ramRigidBody &o = AM.getRigidBody(n);
				scene->drawRigid(o);
			}
			
			ramEndCamera();
		}
	}
}

void ramSceneManager::drawActor(ramActor &actor)
{
	ramBasicActor(actor);
	
	for (int i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->drawActor(actor);
	}
}

void ramSceneManager::drawRigid(ramRigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBeginCamera();
		scenes.at(i)->drawRigid(rigid);
		ramEndCamera();
	}
}