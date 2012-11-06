#include "ramActor.h"

#pragma mark - ramNode

ramNode::ramNode() : ofNode()
{

}

ramNode& ramNode::operator=(const ramNode& copy)
{
	node_id = copy.node_id;
	name = copy.name;
	return *this;
}


#pragma mark - ramNodeArray

ramNodeArray& ramNodeArray::operator=(const ramNodeArray& copy)
{
	name = copy.name;
	nodes = copy.nodes;

	last_timestamp = copy.last_timestamp;
	current_timestamp = copy.current_timestamp;

	last_update_client_time = copy.last_update_client_time;
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
	}

	// クライアントタイムと比較しないとちゃんと動かないかも?
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

ramActor& ramActor::operator=(const ramActor& copy)
{
	ramNodeArray::operator=(copy);
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
	getNode(JOINT_ADBOMEN).setParent(getNode(JOINT_HIPS));
	{
		getNode(JOINT_CHEST).setParent(getNode(JOINT_ADBOMEN));
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