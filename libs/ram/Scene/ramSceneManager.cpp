#include "ramSceneManager.h"

#include "ramGlobal.h"
#include "ramControlPanel.h"

void ramSceneManager::setup(vector<ramSceneBase*>& scenes)
{
	ramGetGUI().setupSceneToggles(scenes);
	
	for (int i=0; i<scenes.size(); i++)
	{
		ofxUICanvas *panel = scenes.at(i)->createScenePanel();
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
	cout << 1;
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
