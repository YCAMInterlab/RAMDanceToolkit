#include "ramActor.h"

#pragma mark - ramNode

string getJointName(unsigned int jointId) { return ramActor::getJointName(jointId); }

ramNode::ramNode() : ofNode(), node_id(-1), container(NULL)
{
}

ramNode& ramNode::operator=(const ramNode& copy)
{
	ofNode::operator=(copy);
	
	node_id = copy.node_id;
	name = copy.name;
	accerelometer = copy.accerelometer;
	parent = copy.parent;
	
	container = NULL;
	
	return *this;
}

void ramNode::drawId(int floatPos)
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getID()), pos);
}

void ramNode::drawName(int floatPos)
{
	ofVec3f pos = getGlobalPosition();
	pos.y += floatPos;
	ofDrawBitmapString(ofToString(getName()), pos);
}


#pragma mark - ramNodeArray

ramNodeArray::ramNodeArray() : last_timestamp(0), current_timestamp(0), last_update_client_time(0)
{
}

void ramNodeArray::rebuildHierarchy(const ramNodeArray& ref)
{
	// rebuild hierarchy
	for (int i = 0; i < ref.nodes.size(); i++)
	{
		const ramNode &src = ref.nodes[i];
		ramNode &dst = nodes[i];
		
		dst.container = this;
		
		ramNode *p = src.getParent();
		if (!p) continue;
		
		int idx = p->getID();
		if (idx < 0) continue;
		
		dst.setParent(nodes[idx]);
	}
}

ramNodeArray& ramNodeArray::operator=(const ramNodeArray& copy)
{
	name = copy.name;
	nodes = copy.nodes;
	type = copy.type;
	
	rebuildHierarchy(copy);
	
	last_timestamp = copy.last_timestamp;
	current_timestamp = copy.current_timestamp;
	last_update_client_time = copy.last_update_client_time;
	
	return *this;
}

ramNodeArray ramNodeArray::operator+(const ramNodeArray &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	ramNodeArray result = *this;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		const ramNode &src = arr.getNode(i);
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() + src.getPosition());
		dst.setOrientation(src.getOrientationQuat() * dst.getOrientationQuat());
	}
	
	return result;
}

ramNodeArray& ramNodeArray::operator+=(const ramNodeArray &arr)
{
	assert(getNumNode() == arr.getNumNode());

	ramNodeArray &result = *this;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		const ramNode &src = arr.getNode(i);
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() + src.getPosition());
		dst.setOrientation(src.getOrientationQuat() * dst.getOrientationQuat());
	}
	
	return result;
}

ramNodeArray ramNodeArray::operator-(const ramNodeArray &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	ramNodeArray result = *this;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		const ramNode &src = arr.getNode(i);
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() - src.getPosition());
		dst.setOrientation(src.getOrientationQuat().inverse() * dst.getOrientationQuat());
	}
	
	return result;
}

ramNodeArray& ramNodeArray::operator-=(const ramNodeArray &arr)
{
	assert(getNumNode() == arr.getNumNode());
	
	ramNodeArray &result = *this;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		const ramNode &src = arr.getNode(i);
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() - src.getPosition());
		dst.setOrientation(src.getOrientationQuat().inverse() * dst.getOrientationQuat());
	}
	
	return result;
}

ramNodeArray ramNodeArray::operator*(float s) const
{
	ramNodeArray result = *this;
	
	static const ofQuaternion norm;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() * s);
		
		ofQuaternion q;
		q.slerp(s, norm, dst.getOrientationQuat());
		dst.setOrientation(q);
	}
	
	return result;
}

ramNodeArray& ramNodeArray::operator*=(float s)
{
	ramNodeArray &result = *this;
	
	static const ofQuaternion norm;
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		ramNode &dst = result.getNode(i);
		
		dst.setPosition(dst.getPosition() * s);
		
		ofQuaternion q;
		q.slerp(s, norm, dst.getOrientationQuat());
		dst.setOrientation(q);
	}
	
	return result;
}

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
		node.accerelometer.update(vec, quat);
	}

	last_timestamp = current_timestamp;
	current_timestamp = m.getArgAsFloat(2 + nNodes * 8);

	last_update_client_time = ofGetElapsedTimef();
}

#pragma mark - ramRigidBody

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
	nodes.resize(NUM_JOINTS);
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
	
	for (int i=0; i<names.size(); i++) names.at(i) = getJointName(i);
	return names;
}

