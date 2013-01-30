#include "ramSceneManager.h"

#include "ramGlobal.h"
#include "ramControlPanel.h"

void ramSceneManager::setup(vector<ramSceneBase*>& scenes)
{
	ramControlPanel &gui = ramGlobal().getGUI();
	
	// !!!:
//	gui.addScenePanels(scenes);
	
	this->scenes = scenes;
}

void ramSceneManager::update()
{
	ramControlPanel &gui = ramGlobal().getGUI();
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramSceneBase *scene = scenes[i];
		string key = scene->getSceneEnableKey();
		
		
		// !!!:
//		if ( gui.hasValueChanged(key) )
//		{
//			scene->setEnabled( gui.getValueB(key) );
//		}

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
		
		scene->draw();
		
		ramCameraBegin();
		
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
		
		ramCameraEnd();
	}
}
