#include "ramSceneManager.h"

#include "ramSharedData.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

extern bool drawModel;

void ramSceneManager::setup(const vector<ramBaseScene*>& scenes_)
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