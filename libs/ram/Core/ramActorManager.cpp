// 
// ramActorManager.cpp - RAMDanceToolkit
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

#include "ramActorManager.h"

#include "ramCameraManager.h"
#include "ramNodeFinder.h"
#include "ramGraphics.h"

using namespace ofxInteractivePrimitives;

#pragma mark - ramActorManager::NodeSelector

class ramActorManager::NodeSelector : public ofxInteractivePrimitives::Node
{
public:

	enum
	{
		NODE_SELECTOR_PREFIX_ID = 1000
	};

	ofEvent<ramNodeIdentifer> selectStateChanged;
	ramNodeIdentifer identifer;

	NodeSelector(RootNode &root) { setParent(&root); }

	void draw()
	{
		ramActorManager &AM = ramActorManager::instance();
		const vector<GLuint> &NS = getCurrentNameStack();

		ofNoFill();

		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			ramNodeArray &NA = AM.getNodeArray(n);

			for (int i = 0; i < NA.getNumNode(); i++)
			{
				ramNode &node = NA.getNode(i);

				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				ramBillboard();

				if (NS.size() == 3
					&& NS[1] == n
					&& NS[2] == i)
				{
					ofSetColor(255, 0, 0);
					ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 20) * 5);
				}

				glPopMatrix();
			}
		}
	}

	void hittest()
	{
		ofFill();

		pushID(NODE_SELECTOR_PREFIX_ID);

		ramActorManager &AM = ramActorManager::instance();
		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			ramNodeArray &NA = AM.getNodeArray(n);

			pushID(n);

			for (int i = 0; i < NA.getNumNode(); i++)
			{
				ramNode &node = NA.getNode(i);

				pushID(i);

				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				ramBillboard();

				ofCircle(0, 0, 15);

				glPopMatrix();

				popID();
			}

			popID();
		}

		popID();
	}

	void mousePressed(int x, int y, int button)
	{
		const vector<GLuint> &NS = getCurrentNameStack();
		ramActorManager &AM = ramActorManager::instance();

		if (NS.size() == 3 && NS[0] == NODE_SELECTOR_PREFIX_ID)
		{
			const ramNodeArray &NA = AM.getNodeArray(NS[1]);
			string name = NA.getName();
			int index = NS[2];

			identifer.name = name;
			identifer.index = index;

			ofNotifyEvent(selectStateChanged, identifer);

			ramEnableInteractiveCamera(false);
		}
		else
		{
			identifer.name = "";
			identifer.index = -1;

			ofNotifyEvent(selectStateChanged, identifer);
		}
	}

	void mouseReleased(int x, int y, int button)
	{
		ramEnableInteractiveCamera(true);
	}

	bool isObjectPickedUp()
	{
		return !getCurrentNameStack().empty();
	}
};

#pragma mark - ramActorManager

ramActorManager* ramActorManager::_instance = NULL;

void ramActorManager::setup()
{
	freeze = false;

	nodeSelector = new NodeSelector(rootNode);

	ofAddListener(nodeSelector->selectStateChanged, this, &ramActorManager::onSelectStateChanged);
	ofAddListener(ofEvents().mouseReleased, this, &ramActorManager::onMouseReleased);
}

void ramActorManager::update()
{
	while (oscReceiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		ramActorManager::instance().updateWithOscMessage(m);
	}

	nodearrays.updateIndexCache();

	for (int i = 0; i < nodearrays.size(); i++)
	{
		const ramNodeArray &array = nodearrays[i];

		if (array.isOutdated() && !isFreezed() && !array.isPlayback())
		{
			if (array.isActor())
			{
				ramActor o = array;
				ofNotifyEvent(actorExit, o);
			}
			else
			{
				ramRigidBody o = array;
				ofNotifyEvent(rigidExit, o);
			}

			nodearrays.erase(array.getName());
		}
	}

	rootNode.update();
}

void ramActorManager::draw()
{
	rootNode.draw();

	if (nodeSelector != NULL && nodeSelector->identifer.isValid())
	{
		ramNode node;
		ramNodeFinder finder(nodeSelector->identifer);

		if (finder.findOne(node))
		{
			node.beginTransform();

			ofPushStyle();
			ramBillboard();

			ofFill();
			ofSetColor(255, 0, 0, 80);
			ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);

			ofNoFill();
			ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);
			ofPopStyle();

			node.endTransform();
		}
	}
}

void ramActorManager::updateWithOscMessage(const ofxOscMessage &m)
{
	if (isFreezed()) return;

	const std::string addr = m.getAddress();
	const std::string name = m.getArgAsString(0);

	if (addr == RAM_OSC_ADDR_ACTOR)
	{
		if (!nodearrays.hasKey(name))
		{
			ramActor o;
			o.setType(RAM_NODEARRAY_TYPE_ACTOR);
			o.setName(name);
			o.updateWithOscMessage(m);
			nodearrays.set(name, o);

			ofNotifyEvent(actorSetup, o);
		}
		else
		{
			ramActor &o = (ramActor &)nodearrays[name];
			o.updateWithOscMessage(m);
		}
	}
	else if (addr == RAM_OSC_ADDR_RIGID_BODY)
	{
		if (!nodearrays.hasKey(name))
		{
			ramRigidBody o;
			o.setType(RAM_NODEARRAY_TYPE_RIGIDBODY);
			o.setName(name);
			o.updateWithOscMessage(m);
			nodearrays.set(name, o);

			ofNotifyEvent(rigidSetup, o);
		}
		else
		{
			ramRigidBody &o = (ramRigidBody &)nodearrays[name];
			o.updateWithOscMessage(m);
		}
	}
    else assert(false);
}

const ramNodeIdentifer& ramActorManager::getLastSelectedNodeIdentifer()
{
	return nodeSelector->identifer;
}

const ramNode* ramActorManager::getLastSelectedNode()
{
	const ramNodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;

	return &getNodeArray(node_id.name).getNode(node_id.index);
}

const ramNodeArray* ramActorManager::getLastSelectedNodeArray()
{
	const ramNodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;

	return &getNodeArray(node_id.name);
}

void ramActorManager::onSelectStateChanged(ramNodeIdentifer &e)
{
	ofNotifyEvent(selectStateChanged, e);
}

void ramActorManager::onMouseReleased(ofMouseEventArgs &e)
{
	if (!rootNode.hasFocusdObject())
		nodeSelector->identifer.clear();
}

void ramActorManager::clearSelected()
{
	nodeSelector->identifer.clear();
}
