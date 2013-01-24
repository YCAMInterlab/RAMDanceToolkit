#include "ramActor.h"

#pragma mark - ramNode

string getJointName(unsigned int jointId) { return ramActor::getJointName(jointId); }

ramNode::ramNode() : ofNode(), parent(NULL), node_id(-1)
{
}

ramNode& ramNode::operator=(const ramNode& copy)
{
	ofNode::operator=(copy);
	
	node_id = copy.node_id;
	name = copy.name;
	accerelometer = copy.accerelometer;
	
	parent = NULL;
	return *this;
}


#pragma mark - ramNodeArray

ramNodeArray::ramNodeArray() : last_timestamp(0), current_timestamp(0), last_update_client_time(0)
{
}

ramNodeArray& ramNodeArray::operator=(const ramNodeArray& copy)
{
	name = copy.name;
	nodes = copy.nodes;
	
	// rebuild hierarchy
	for (int i = 0; i < copy.nodes.size(); i++)
	{
		const ramNode &src = copy.nodes[i];
		ramNode &dst = nodes[i];
		
		ramNode *p = src.getParent();
		if (!p) continue;
		
		int idx = p->getID();
		if (idx < 0) continue;
		
		dst.setParent(nodes[idx]);
	}
	
	last_timestamp = copy.last_timestamp;
	current_timestamp = copy.current_timestamp;

	last_update_client_time = copy.last_update_client_time;
	return *this;
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
		node.name = name;

		node.setPosition(vec);
		node.setOrientation(quat);
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

#pragma mark - ramActor

ramActor::ramActor()
{
	nodes.resize(NUM_JOINTS);
	setupTree();
}

ramActor::~ramActor()
{
	dispose();
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
	"JOINT_HIPS",
	"JOINT_ABDOMEN",
	"JOINT_CHEST",
	"JOINT_NECK",
	"JOINT_HEAD",
	"JOINT_LEFT_HIP",
	"JOINT_LEFT_KNEE",
	"JOINT_LEFT_ANKLE",
	"JOINT_LEFT_TOE",
	"JOINT_RIGHT_HIP",
	"JOINT_RIGHT_KNEE",
	"JOINT_RIGHT_ANKLE",
	"JOINT_RIGHT_TOE",
	"JOINT_LEFT_COLLAR",
	"JOINT_LEFT_SHOULDER",
	"JOINT_LEFT_ELBOW",
	"JOINT_LEFT_WRIST",
	"JOINT_LEFT_HAND",
	"JOINT_RIGHT_COLLAR",
	"JOINT_RIGHT_SHOULDER",
	"JOINT_RIGHT_ELBOW",
	"JOINT_RIGHT_WRIST",
	"JOINT_RIGHT_HAND"
};

vector<string> ramActor::getJointNames()
{
	vector<string> names;
	names.clear();
	names.resize(ramActor::NUM_JOINTS);
	
	for (int i=0; i<names.size(); i++) names.at(i) = getJointName(i);
	return names;
}

