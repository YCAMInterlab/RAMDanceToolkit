#include "ramSceneManager.h"

#include "ramControlPanel.h"
#include "ramPhysics.h"

extern bool drawModel;

void ramSceneManager::setup(const vector<ramBaseScene*>& scenes_)
{
	enableAllEvents();
	
	scenes = scenes_;

	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes.at(i);
		scene->setup();
		ramGetGUI().addPanel(scene);
	}
	
	// bomisutaro: this is a bit dangerous because the scene manager is not a singleton
	ramBaseScene *actorsScene = ramGetGUI().getActorsScene();
	scenes.push_back(actorsScene);
	actorsScene->setup();
}

void ramSceneManager::update()
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

void ramSceneManager::draw()
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