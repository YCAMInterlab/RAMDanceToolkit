#include "ramSceneManager.h"

#include "ramSharedData.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

void ramSceneManager::setup(vector<ramBaseScene*>& scenes_)
{
	scenes = scenes_;
	
	ramGetGUI().setupSceneToggles(scenes);
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes.at(i);
		scene->setup();
		ramGetGUI().addPanel(scene);
	}
}

void ramSceneManager::update()
{
	for (int i=0; i<scenes.size(); i++)
	{
		if (i >= scenes.size()) break;
		
		ramBaseScene *scene = scenes.at(i);
		if (scene->isEnabled())
			scene->update();
		else
			continue;
	}
}

void ramSceneManager::draw()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;

		bool enable_physics = ramGetEnablePhysicsPrimitive();
		ramSetEnablePhysicsPrimitive(false);
		
		{
			// draw shadow
			
			ramBeginShadow();
			
			scene->draw();
			
			ramBeginCamera();
			
			for (int n = 0; n < getNumNodeArray(); n++)
			{
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
			}
			
			ramEndCamera();
			
			ramEndShadow();
		}

		ramSetEnablePhysicsPrimitive(enable_physics);
		
		{
			// draw object
			
			scene->draw();
			
			ramBeginCamera();
			
			for (int n = 0; n < getNumNodeArray(); n++)
			{
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
			}
			
			ramEndCamera();
		}
	}
}

void ramSceneManager::drawActor(ramActor &actor)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		scene->drawActor(actor);
	}
}

void ramSceneManager::drawRigid(ramRigidBody &rigid)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
		
		scene->drawRigid(rigid);
	}
}