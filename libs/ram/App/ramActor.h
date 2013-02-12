#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramConstants.h"

class ramNodeArray;
class ramAccelerometer;


#pragma mark - ramAccelerometer

class ramAccelerometer
{
	friend class ramNode;
	
public:
	ramAccelerometer()
	{
		acceleration.set(0, 0, 0);
		angular_velocity.set(0, 0, 0, 1);
		last_pos.set(0, 0, 0);
		last_rot.set(0, 0, 0, 1);
	}
	virtual ~ramAccelerometer() {}
	
	void update(const ofVec3f &pos, const ofQuaternion &quat)
	{
		velocity = last_pos - pos;
		last_pos = pos;
		
		acceleration = last_velocity - velocity;
		last_velocity = velocity;
		
		angular_velocity = last_rot.inverse() * quat;
		last_rot = quat;
		
		angular_acceleration = last_angular_velocity.inverse() * angular_velocity;
		last_angular_velocity = angular_velocity;
	}
	
private:
	ofVec3f velocity, last_velocity, acceleration;
	ofQuaternion angular_velocity, last_angular_velocity, angular_acceleration;
	
	ofVec3f last_pos;
	ofQuaternion last_rot;
};


#pragma mark - ramNode

class ramNode : public ofNode
{
	friend class ramNodeArray;

public:

	ramNode();
	ramNode(const ramNode& copy) { *this = copy; }
	ramNode& operator=(const ramNode& copy);
	const string& getName() { return name; }
	int getID() { return node_id; }

	inline void setParent(ramNode &parent) { this->parent = &parent; }
	inline ramNode* getParent() const { return parent; }
	inline bool hasParent() const { return parent != NULL; }

	// utils
	inline void transformBegin() const { transformGL(); }
	inline void transformEnd() const { restoreTransformGL(); }
	
	const ofMatrix4x4& getTransformMatrix() const { return getLocalTransformMatrix(); }
	const ofMatrix4x4& getMatrix() const { return getLocalTransformMatrix(); }
	
	inline ofVec3f getVelocity() { return accerelometer.velocity; }
	inline ofVec3f getAcceleration() { return accerelometer.acceleration; }
	inline ofQuaternion getAngularVelocity() { return accerelometer.angular_velocity; }
	inline ofQuaternion getAngularAcceleration() { return accerelometer.angular_acceleration; }
	
	operator ofVec3f() const { return getGlobalPosition(); }
	
	inline ramAccelerometer& getAccerelometer() { return accerelometer; }
	
private:

	int node_id;
	string name;

	ramNode *parent;
	ramAccelerometer accerelometer;
	
};

#pragma mark - ramNodeArray

class ramNodeArray
{
public:

	ramNodeArray();
	ramNodeArray(const ramNodeArray& copy) { *this = copy; }
	ramNodeArray& operator=(const ramNodeArray& copy);
	virtual ~ramNodeArray() {}

	void setName(const string& name) { this->name = name; }
	string& getName() { return name; }

	int getNumNode() { return nodes.size(); }
	ramNode& getNode(int node_id) { return nodes[node_id]; }
	
	inline bool isOutdated() { return (ofGetElapsedTimef() -  last_update_client_time) > RAM_OUTDATED_DURATION; }
	inline float getTimestamp() { return last_update_client_time; }
	
	virtual void updateWithOscMessage(const ofxOscMessage &m);

protected:

	string name;
	vector<ramNode> nodes;

	float last_timestamp;
	float current_timestamp;

	float last_update_client_time;
};

#pragma mark - ramRigidBody

class ramRigidBody : public ramNodeArray
{
    
public:

	virtual void updateWithOscMessage(const ofxOscMessage &m);

private:
	
	void reserveNodes(int num);
};

#pragma mark - ramActor

class ramActor : public ramNodeArray
{
public:

	enum Joint
	{
		JOINT_HIPS              = 0,
		JOINT_ABDOMEN           = 1,
		JOINT_CHEST             = 2,
		JOINT_NECK              = 3,
		JOINT_HEAD              = 4,

		JOINT_LEFT_HIP          = 5,
		JOINT_LEFT_KNEE         = 6,
		JOINT_LEFT_ANKLE        = 7,
		JOINT_LEFT_TOE          = 8,

		JOINT_RIGHT_HIP         = 9,
		JOINT_RIGHT_KNEE        = 10,
		JOINT_RIGHT_ANKLE       = 11,
		JOINT_RIGHT_TOE         = 12,

		JOINT_LEFT_COLLAR       = 13,
		JOINT_LEFT_SHOULDER     = 14,
		JOINT_LEFT_ELBOW        = 15,
		JOINT_LEFT_WRIST        = 16,
		JOINT_LEFT_HAND         = 17,

		JOINT_RIGHT_COLLAR      = 18,
		JOINT_RIGHT_SHOULDER    = 19,
		JOINT_RIGHT_ELBOW       = 20,
		JOINT_RIGHT_WRIST       = 21,
		JOINT_RIGHT_HAND        = 22,

		NUM_JOINTS              = 23,
	};

	ramActor();
	virtual ~ramActor();

	virtual void updateWithOscMessage(const ofxOscMessage &m);
	static string getJointName(int jointId) { return jointName[jointId]; }
	static vector<string> getJointNames();
	
	
private:
	static string jointName[NUM_JOINTS];
	void dispose();

	void setupTree();
};

