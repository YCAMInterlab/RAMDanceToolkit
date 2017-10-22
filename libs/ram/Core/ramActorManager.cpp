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

//using namespace ofxInteractivePrimitives;
using namespace rdtk;

#pragma mark - ActorManager::NodeSelector

class ActorManager::NodeSelector : public ofxInteractivePrimitives::Node
{
public:

	enum
	{
		NODE_SELECTOR_PREFIX_ID = 1000
	};

	ofEvent<NodeIdentifer> selectStateChanged;
	NodeIdentifer identifer;

	NodeSelector(ofxInteractivePrimitives::RootNode &root) { setParent(&root); }

	void draw()
	{
		ActorManager &AM = ActorManager::instance();
		const vector<GLuint> &NS = getCurrentNameStack();

		ofNoFill();

		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			rdtk::NodeArray &NA = AM.getNodeArray(n);

			for (int i = 0; i < NA.getNumNode(); i++)
			{
				rdtk::Node &node = NA.getNode(i);

				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				Billboard();

				if (NS.size() == 3
					&& NS[1] == n
					&& NS[2] == i)
				{
					ofSetColor(255, 0, 0);
					ofDrawCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 20) * 5);
				}

				glPopMatrix();
			}
		}
	}

	void hittest()
	{
		ofFill();

		pushID(NODE_SELECTOR_PREFIX_ID);

		ActorManager &AM = ActorManager::instance();
		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			NodeArray &NA = AM.getNodeArray(n);

			pushID(n);

			for (int i = 0; i < NA.getNumNode(); i++)
			{
				rdtk::Node &node = NA.getNode(i);

				pushID(i);

				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				Billboard();

				ofDrawCircle(0, 0, 15);

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
		ActorManager &AM = ActorManager::instance();

		if (NS.size() == 3 && NS[0] == NODE_SELECTOR_PREFIX_ID)
		{
			const NodeArray &NA = AM.getNodeArray(NS[1]);
			string name = NA.getName();
			int index = NS[2];

			identifer.name = name;
			identifer.index = index;

			ofNotifyEvent(selectStateChanged, identifer);

			EnableInteractiveCamera(false);
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
		EnableInteractiveCamera(true);
	}

	bool isObjectPickedUp()
	{
		return !getCurrentNameStack().empty();
	}
};

#pragma mark - ActorManager

ActorManager* ActorManager::_instance = NULL;

void ActorManager::setup()
{
	freeze = false;

	nodeSelector = new NodeSelector(rootNode);

	ofAddListener(nodeSelector->selectStateChanged, this, &ActorManager::onSelectStateChanged);
	ofAddListener(ofEvents().mouseReleased, this, &ActorManager::onMouseReleased);
}

void ActorManager::update()
{
	while (oscReceiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		ActorManager::instance().updateWithOscMessage(m);
	}

	nodearrays.updateIndexCache();

	for (int i = 0; i < nodearrays.size(); i++)
	{
		const ofPtr<NodeArray> & array = nodearrays[i];

		if (array->isOutdated() && !isFreezed() && !array->isPlayback())
		{
			if (array->isActor())
			{
				Actor o = *array.get();
				ofNotifyEvent(actorExit, o);
			}
			else
			{
				RigidBody o = *array.get();
				ofNotifyEvent(rigidExit, o);
			}

			nodearrays.erase(array->getName());
		}
	}

	rootNode.update();
}

void ActorManager::draw()
{
	rootNode.draw();

	if (nodeSelector != NULL && nodeSelector->identifer.isValid())
	{
		Node node;
		NodeFinder finder(nodeSelector->identifer);

		if (finder.findOne(node))
		{
			node.beginTransform();

			ofPushStyle();
			Billboard();

			ofFill();
			ofSetColor(255, 0, 0, 80);
			ofDrawCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);

			ofNoFill();
			ofDrawCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);
			ofPopStyle();

			node.endTransform();
		}
	}
}

void ActorManager::updateWithOscMessage(const ofxOscMessage &m)
{
	if (isFreezed()) return;

	const std::string addr = m.getAddress();
	const std::string name = m.getArgAsString(0);

	if (addr == RAM_OSC_ADDR_ACTOR)
	{
		if (!nodearrays.hasKey(name))
		{
			ofPtr<Actor> o = make_shared<Actor>();
			o->setType(RAM_NODEARRAY_TYPE_ACTOR);
			o->setName(name);
			o->updateWithOscMessage(m);
			nodearrays.set(name, o);
			
			ofNotifyEvent(actorSetup, *o.get());
		}
		else
		{
			ofPtr<Actor> o = static_pointer_cast<Actor>(nodearrays[name]);
			o->updateWithOscMessage(m);
		}
	}
	else if (addr == RAM_OSC_ADDR_RIGID_BODY)
	{
		if (!nodearrays.hasKey(name))
		{
			ofPtr<RigidBody> o = make_shared<RigidBody>();
			o->setType(RAM_NODEARRAY_TYPE_RIGIDBODY);
			o->setName(name);
			o->updateWithOscMessage(m);
			nodearrays.set(name, o);

			ofNotifyEvent(rigidSetup, *o.get());
		}
		else
		{
			ofPtr<RigidBody> o = static_pointer_cast<RigidBody>(nodearrays[name]);
			o->updateWithOscMessage(m);
		}
	}
    else assert(false);
}

const NodeIdentifer& ActorManager::getLastSelectedNodeIdentifer() const
{
	return nodeSelector->identifer;
}

const Node* ActorManager::getLastSelectedNode() const
{
	const NodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;

	return &getNodeArray(node_id.name).getNode(node_id.index);
}

const NodeArray* ActorManager::getLastSelectedNodeArray() const
{
	const NodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;

	return &getNodeArray(node_id.name);
}

void ActorManager::onSelectStateChanged(NodeIdentifer &e)
{
	ofNotifyEvent(selectStateChanged, e);
}

void ActorManager::onMouseReleased(ofMouseEventArgs &e)
{
	if (!rootNode.hasFocusObject())
		nodeSelector->identifer.clear();
}

void ActorManager::clearSelected()
{
	nodeSelector->identifer.clear();
}

void ActorManager::setEnableOnMouseRelease(bool v)
{
	if (v)
		ofAddListener(ofEvents().mouseReleased, this, &ActorManager::onMouseReleased);
	else
		ofRemoveListener(ofEvents().mouseReleased, this, &ActorManager::onMouseReleased);
}
