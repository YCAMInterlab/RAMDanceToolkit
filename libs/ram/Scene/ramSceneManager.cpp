#include "ramSceneManager.h"

#include "ramSharedData.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

void ramSceneManager::setup(vector<ramSceneBase*>& scenes)
{
	ramGetGUI().setupSceneToggles(scenes);
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramSceneBase *scene = scenes.at(i);
		scene->setup();
		
		ofxUICanvas *panel = scene->createScenePanel();
		ramGetGUI().getTabbedCanvas().add(panel);
	}
	
	this->scenes = scenes;
}

void ramSceneManager::update()
{
	ramControlPanel &gui = ramGetGUI();
	
	vector<ofxUIToggle *> toggles = gui.getSceneToggles()->getToggles();
	const int numToggles = toggles.size();
	
	for (int i=0; i<numToggles; i++)
	{
		if (i >= scenes.size()) break;
		
		ramSceneBase *scene = scenes.at(i);
		scene->setEnabled( toggles.at(i)->getValue() );
		
		if (!scene->isEnabled()) continue;
		scene->update();
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
