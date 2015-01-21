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
#include "dpConstants.h"

extern bool drawModel;

ramSceneManager* ramSceneManager::_instance = NULL;

ramSceneManager& ramSceneManager::instance()
{
	if (_instance == NULL)
		_instance = new ramSceneManager;
	return *_instance;
}

ramSceneManager::ramSceneManager() :
isAllocatedFbos(false)
{

}

ramSceneManager::ramSceneManager(const ramSceneManager&) {}

ramSceneManager& ramSceneManager::operator=(const ramSceneManager&) { return *this; }

void ramSceneManager::setup()
{
	enableAllEvents();
	
	ofAddListener(ofEvents().update, this, &ramSceneManager::update);
	ofAddListener(ofEvents().draw, this, &ramSceneManager::draw);
	ofAddListener(ofEvents().exit, this, &ramSceneManager::exit);
	
	actorsScene = new ramActorsScene();
    scenes.push_back(actorsScene);
    actorsScene->setup();
    actorsScene->setEnabled(true);
    ramGetGUI().addPanel(actorsScene, false);
    
    vector<bool> screenStates;
    screenStates.assign(NUM_SCREEN_POSITIONS, true);
    screens.push_back(screenStates);
    
    oscReceiverTag.addAddress(RAM_OSC_ADDR_SET_SCENE);
    ramOscManager::instance().addReceiverTag(&oscReceiverTag);
}

void ramSceneManager::addScene(ramBaseScene* scene)
{
	scenes.push_back(scene);
	scene->setup();
	ramGetGUI().addPanel(scene);
    
    vector<bool> screenStates;
    screenStates.assign(NUM_SCREEN_POSITIONS, true);
    screens.push_back(screenStates);
    cout << scene->getName() << ", " << endl;
}

void ramSceneManager::allocateFbos(int w, int h)
{
    for (int i=0; i<scenes.size(); i++) {
        scenes.at(i)->allocateFbo(w, h);
    }
    isAllocatedFbos = true;
}

void ramSceneManager::deallocateFbos()
{
    for (int i=0; i<scenes.size(); i++) {
        scenes.at(i)->deallocateFbo();
    }
    isAllocatedFbos = false;
}

void ramSceneManager::setScreen(int sceneId, int screenId, bool enabled)
{
    if (sceneId < 0) return;
    vector<bool>& screenStates = screens.at(sceneId);
    if (screenId >= 0 && screenId < screenStates.size()) {
        screenStates.at(screenId) = enabled;
    }
}

void ramSceneManager::setAllScreens(int sceneId, bool enabled)
{
    if (sceneId < 0) return;
    vector<bool>& screenStates = screens.at(sceneId);
    for (int i=0; i<screenStates.size(); i++) {
        screenStates.at(i) = enabled;
    }
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
ramBaseScene* ramSceneManager::getScene(size_t index)
{
	return scenes.at(index);
}

ramActorsScene* ramSceneManager::getActorsScene()
{
	return actorsScene;
}

void ramSceneManager::setShowAllActors(bool showAllActors)
{
	actorsScene->showAll(showAllActors);
}

bool ramSceneManager::getShowAllActors() const
{
	return actorsScene->getShowAll();
}

void ramSceneManager::update(ofEventArgs& args)
{
    updateOsc();
    
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

void ramSceneManager::updateOsc()
{
    while (oscReceiverTag.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiverTag.getNextMessage(&m);
        const string& addr = m.getAddress();
        try {
            if (addr == RAM_OSC_ADDR_SET_SCENE) {
                const string header = "set_scene: ";
                const int numArgs = 4;
                if (!(m.getNumArgs() == numArgs))
                    throw runtime_error(header + "incorrect number of arguments " + ofToString(m.getNumArgs()));
                
                if (!(m.getArgType(0) == OFXOSC_TYPE_STRING))
                    throw runtime_error(header + "incorrect type of argument 0");

                for (int i=1; i<numArgs; i++) {
                    if (!(m.getArgType(i) == OFXOSC_TYPE_INT32))
                        throw runtime_error(header + "incorrect type of argument " + ofToString(i));
                }
                
                const string sceneName = m.getArgAsString(0);
                const bool enabled = (bool)m.getArgAsInt32(1);
                const bool screen0 = (bool)m.getArgAsInt32(2);
                const bool screen1 = (bool)m.getArgAsInt32(3);
                
                const int sceneIdx = findtSceneIndex(sceneName);
                if (sceneIdx < 0)
                    throw runtime_error(header + "no scene " + sceneName + " found");

                if (enabled) ramGetGUI().getSceneTabs().select(sceneName);
				getScene(sceneIdx)->setEnabled(enabled);
                setScreen(sceneIdx, 0, screen0);
                setScreen(sceneIdx, 1, screen1);
                
                ofLogNotice("ramSceneManager") << boolalpha << header << sceneName << ", " << enabled << ", " << screen0 << ", " << screen1;
            }
        }
        catch(exception& e) {
            ofLogError("ramSceneManager") << e.what();
        }
    }
}

void ramSceneManager::draw(ofEventArgs& args)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	ofPushStyle();

	glEnable(GL_DEPTH_TEST);
    
    map<int, ramBaseScene*> fboRenderd;

	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		if (!scene->isEnabled()) continue;
        
        if (isAllocatedFbos && scene->isAllocatedFbo()) {
            scene->getFbo()->begin();
            
            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

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
        
        if (isAllocatedFbos && scene->isAllocatedFbo()) {
            scene->getFbo()->end();
            fboRenderd[i] = scene;
        }
	}
    
	ofPopStyle();
	glPopMatrix();
	glPopAttrib();

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white);
    map<int, ramBaseScene*>::iterator it;
    for (it = fboRenderd.begin(); it!=fboRenderd.end(); ++it) {
        ramBaseScene* scene = it->second;
        vector<bool> screenStates = screens.at(it->first);
        for (int i=0; i<screenStates.size(); i++) {
            if (screenStates.at(i)) {
                scene->getFbo()->draw(SCREEN_POSITIONS[i],
                                      scene->getFbo()->getWidth(),
                                      scene->getFbo()->getHeight());
            }
        }
    }
    ofPopStyle();
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