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

string getJointName(unsigned int jointId) { return ramActor::getJointName(jointId); }

ramNode& ramNode::operator=(const ramNode& copy)
{
	ofxNodeArray::Node<ramNode>::operator=(copy);

	name = copy.name;
	accelerometer = copy.accelerometer;

	return *this;
}

void ramNode::drawNodeId(int floatPos) const
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getID()), pos);
}

void ramNode::drawNodeName(int floatPos) const
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getName()), pos);
}

#pragma mark - ramNodeArray

ramNodeArray::ramNodeArray() : last_timestamp(0), current_timestamp(0), last_update_client_time(0), is_playback(false)
{
}

ramNodeArray& ramNodeArray::operator=(const ramNodeArray& copy)
{
	ofxNodeArray::NodeArray<ramNodeArray, ramNode>::operator=(copy);
	
	type = copy.type;
    
    is_playback = copy.is_playback;
	last_timestamp = copy.last_timestamp;
	current_timestamp = copy.current_timestamp;
	last_update_client_time = copy.last_update_client_time;

	return *this;
}

//

void ramNodeArray::updateWithOscMessage(const ofxOscMessage &m)
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

		ramNode &node = getNode(i);
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

ofPoint ramNodeArray::getCentroid() const{
    
    ofPoint centroid(0,0,0);
    
    for (int i = 0; i < getNumNode(); i++)
    {
        const ramNode &node = getNode(i);
        centroid += node.getGlobalPosition();
    }
    
    centroid /= getNumNode();
    
    return centroid;
    
}

#pragma mark - ramRigidBody

ramRigidBody::ramRigidBody()
{
	type = RAM_NODEARRAY_TYPE_ACTOR;
}

void ramRigidBody::updateWithOscMessage(const ofxOscMessage &m)
{
	const int nNodes = m.getArgAsInt32(1);

	if (nNodes != getNumNode())
		reserveNodes(nNodes);

	ramNodeArray::updateWithOscMessage(m);
}

void ramRigidBody::reserveNodes(int num)
{
	nodes.resize(num);
}

ramRigidBody& ramRigidBody::operator=(const ramNodeArray &copy)
{
	this->ramNodeArray::operator=(copy);
	return *this;
}

#pragma mark - ramActor

ramActor::ramActor()
{
	type = RAM_NODEARRAY_TYPE_ACTOR;
	
	nodes.resize(NUM_JOINTS);
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].node_id = i;
	}
	
	setupTree();
}

ramActor::ramActor(const ramNodeArray &copy)
{
	*this = copy;
}

ramActor::~ramActor()
{
	dispose();
}

ramActor& ramActor::operator=(const ramNodeArray &copy)
{
	this->ramNodeArray::operator=(copy);
	return *this;
}

void ramActor::dispose()
{
	nodes.clear();
}

void ramActor::updateWithOscMessage(const ofxOscMessage &m)
{
	ramNodeArray::updateWithOscMessage(m);
}

void ramActor::setupTree()
{
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

string ramActor::jointName[NUM_JOINTS] =
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

vector<string> ramActor::getJointNames()
{
	vector<string> names;
	names.clear();
	names.resize(ramActor::NUM_JOINTS);

	for (int i = 0; i < names.size(); i++)
		names.at(i) = getJointName(i);
	return names;
}
