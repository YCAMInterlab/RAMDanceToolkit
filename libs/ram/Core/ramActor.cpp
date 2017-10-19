// 
// ramActor.cpp - RAMDanceToolkit
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

#include "ramActor.h"

#pragma mark - ramNode

using namespace rdtk;

string getJointName(unsigned int jointId) { return Actor::getJointName(jointId); }

Node& Node::operator=(const Node& copy)
{
#if defined(OF_VERSION_MAJOR) && defined (OF_VERSION_MINOR)
#if (OF_VERSION_MAJOR == 0) && (OF_VERSION_MINOR <= 8)
	ofxNodeArray::Node<Node>::operator=(copy);
#elif (OF_VERSION_MAJOR == 0) && (OF_VERSION_MINOR == 9)
	//
	// Hotfix for oF v0.9.8 : avoid ofNode.clearParent() from original ofNode destructor.
	// https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/3d/ofNode.cpp#L30
	//
	setPosition(copy.getPosition());
	setOrientation(copy.getOrientationQuat());
	setScale(copy.getScale());
	updateAxis();
	createMatrix();
	node_id = copy.node_id;
	parent = copy.parent;
#endif
#endif
	
	name = copy.name;
	accelerometer = copy.accelerometer;

	return *this;
}

void Node::drawNodeId(int floatPos) const
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getID()), pos);
}

void Node::drawNodeName(int floatPos) const
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getName()), pos);
}

#pragma mark - NodeArray

NodeArray::NodeArray() : last_timestamp(0), current_timestamp(0), last_update_client_time(0), is_playback(false)
{
}

NodeArray& NodeArray::operator=(const NodeArray& copy)
{
	ofxNodeArray::NodeArray<NodeArray, Node>::operator=(copy);
	
	type = copy.type;
    
    is_playback = copy.is_playback;
	last_timestamp = copy.last_timestamp;
	current_timestamp = copy.current_timestamp;
	last_update_client_time = copy.last_update_client_time;

	return *this;
}

//

void NodeArray::updateWithOscMessage(const ofxOscMessage &m)
{
	const int nNodes = m.getArgAsInt32(1);
	
	for (int i = 0; i < nNodes; i++)
	{
		const string name = m.getArgAsString(i * 8 + 0 + 2);
		const float vx = m.getArgAsFloat(i * 8 + 1 + 2);
		const float vy = m.getArgAsFloat(i * 8 + 2 + 2);
		const float vz = m.getArgAsFloat(i * 8 + 3 + 2);
		const float angle = m.getArgAsFloat(i * 8 + 4 + 2);
		const float ax = m.getArgAsFloat(i * 8 + 5 + 2);
		const float ay = m.getArgAsFloat(i * 8 + 6 + 2);
		const float az = m.getArgAsFloat(i * 8 + 7 + 2);
		const ofVec3f axis(ax, ay, az);
		const ofVec3f vec(vx, vy, vz);
		const ofQuaternion quat(angle, axis);
		
		rdtk::Node &node = getNode(i);
		node.node_id = i;
		node.name = isActor() ? getJointName(i) : "Node " + ofToString(i);
		node.setGlobalPosition(vec);
		node.setGlobalOrientation(quat);
		node.accelerometer.update(vec, quat);
	}

	last_timestamp = current_timestamp;
	current_timestamp = m.getArgAsFloat(2 + nNodes * 8);
	last_update_client_time = ofGetElapsedTimef();
}

ofPoint NodeArray::getCentroid() const{
    
    ofPoint centroid(0,0,0);
    
    for (int i = 0; i < getNumNode(); i++)
    {
        const Node &node = getNode(i);
        centroid += node.getGlobalPosition();
    }
    
    centroid /= getNumNode();
    
    return centroid;
    
}

#pragma mark - RigidBody

RigidBody::RigidBody()
{
	type = RAM_NODEARRAY_TYPE_ACTOR;
}

void RigidBody::updateWithOscMessage(const ofxOscMessage &m)
{
	const int nNodes = m.getArgAsInt32(1);

	if (nNodes != getNumNode())
		reserveNodes(nNodes);

	NodeArray::updateWithOscMessage(m);
}

void RigidBody::reserveNodes(int num)
{
	nodes.resize(num);
}

RigidBody& RigidBody::operator=(const NodeArray &copy)
{
	this->NodeArray::operator=(copy);
	return *this;
}

#pragma mark - Actor

Actor::Actor()
{
	type = RAM_NODEARRAY_TYPE_ACTOR;
	
	nodes.resize(NUM_JOINTS);
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].node_id = i;
	}
	
	setupTree();
}

Actor::Actor(const NodeArray &copy)
{
	*this = copy;
	setupTree();
}

Actor::~Actor()
{
	dispose();
}

Actor& Actor::operator=(const NodeArray &copy)
{
	this->NodeArray::operator=(copy);
	return *this;
}

void Actor::dispose()
{
	for (auto & n : nodes) n.clearParent();
	nodes.clear();
}

void Actor::updateWithOscMessage(const ofxOscMessage &m)
{
	if (!getNode(JOINT_ABDOMEN).hasParent()) setupTree();
	NodeArray::updateWithOscMessage(m);
}

void Actor::setupTree()
{
	if (getNumNode() < NUM_JOINTS) return;
	for (auto & n : nodes) n.clearParent();
	
	getNode(JOINT_ABDOMEN).setParent(getNode(JOINT_HIPS));
	{
		getNode(JOINT_CHEST).setParent(getNode(JOINT_ABDOMEN));
		{
			getNode(JOINT_NECK).setParent(getNode(JOINT_CHEST));
			{
				getNode(JOINT_HEAD).setParent(getNode(JOINT_NECK));
			}
		}
		getNode(JOINT_LEFT_COLLAR).setParent(getNode(JOINT_CHEST));
		{
			getNode(JOINT_LEFT_SHOULDER).setParent(getNode(JOINT_LEFT_COLLAR));
			{
				getNode(JOINT_LEFT_ELBOW).setParent(getNode(JOINT_LEFT_SHOULDER));
				{
					getNode(JOINT_LEFT_WRIST).setParent(getNode(JOINT_LEFT_ELBOW));
					{
						getNode(JOINT_LEFT_HAND).setParent(getNode(JOINT_LEFT_WRIST));
					}
				}

			}
		}

		getNode(JOINT_RIGHT_COLLAR).setParent(getNode(JOINT_CHEST));
		{
			getNode(JOINT_RIGHT_SHOULDER).setParent(getNode(JOINT_RIGHT_COLLAR));
			{
				getNode(JOINT_RIGHT_ELBOW).setParent(getNode(JOINT_RIGHT_SHOULDER));
				{
					getNode(JOINT_RIGHT_WRIST).setParent(getNode(JOINT_RIGHT_ELBOW));
					{
						getNode(JOINT_RIGHT_HAND).setParent(getNode(JOINT_RIGHT_WRIST));
					}
				}
			}
		}
	}

	getNode(JOINT_LEFT_HIP).setParent(getNode(JOINT_HIPS));
	{

		getNode(JOINT_LEFT_KNEE).setParent(getNode(JOINT_LEFT_HIP));
		{
			getNode(JOINT_LEFT_ANKLE).setParent(getNode(JOINT_LEFT_KNEE));
			{
				getNode(JOINT_LEFT_TOE).setParent(getNode(JOINT_LEFT_ANKLE));
			}
		}
	}

	getNode(JOINT_RIGHT_HIP).setParent(getNode(JOINT_HIPS));
	{
		getNode(JOINT_RIGHT_KNEE).setParent(getNode(JOINT_RIGHT_HIP));
		{
			getNode(JOINT_RIGHT_ANKLE).setParent(getNode(JOINT_RIGHT_KNEE));
			{
				getNode(JOINT_RIGHT_TOE).setParent(getNode(JOINT_RIGHT_ANKLE));
			}
		}
	}
}

string Actor::jointName[NUM_JOINTS] =
{
	"HIPS",
	"ABDOMEN",
	"CHEST",
	"NECK",
	"HEAD",
	"LEFT_HIP",
	"LEFT_KNEE",
	"LEFT_ANKLE",
	"LEFT_TOE",
	"RIGHT_HIP",
	"RIGHT_KNEE",
	"RIGHT_ANKLE",
	"RIGHT_TOE",
	"LEFT_COLLAR",
	"LEFT_SHOULDER",
	"LEFT_ELBOW",
	"LEFT_WRIST",
	"LEFT_HAND",
	"RIGHT_COLLAR",
	"RIGHT_SHOULDER",
	"RIGHT_ELBOW",
	"RIGHT_WRIST",
	"RIGHT_HAND"
};

vector<string> Actor::getJointNames()
{
	vector<string> names;
	names.clear();
	names.resize(Actor::NUM_JOINTS);

	for (int i = 0; i < names.size(); i++)
		names.at(i) = getJointName(i);
	return names;
}
