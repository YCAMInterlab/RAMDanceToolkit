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

ramActorsScene* ramSceneManager::getActorsScene()
{
	return actorsScene;
}

void ramSceneManager::setShowAllActors(bool showAllActors)
{
	return actorsScene->setShowAll(showAllActors);
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