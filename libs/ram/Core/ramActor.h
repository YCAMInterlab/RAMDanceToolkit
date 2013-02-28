#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxNodeArray.h"

#include "ramConstants.h"

#include <assert.h>

class ramNodeArray;

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

class ramNode : public ofxNodeArray::Node<ramNode>
{
	friend class ramNodeArray;

public:

	ramNode() {}
	ramNode(int index) : Node(index) {}
	
	ramNode(const ramNode& copy) { *this = copy; }
	ramNode& operator=(const ramNode& copy);

	const string& getName() const { return name; }
	
	const ofMatrix4x4& getTransformMatrix() const { return getLocalTransformMatrix(); }
	const ofMatrix4x4& getMatrix() const { return getLocalTransformMatrix(); }

	inline ofVec3f getVelocity() const { return accerelometer.velocity; }
	inline ofVec3f getAcceleration() const { return accerelometer.acceleration; }
	inline ofQuaternion getAngularVelocity() const { return accerelometer.angular_velocity; }
	inline ofQuaternion getAngularAcceleration() const { return accerelometer.angular_acceleration; }

	inline ramAccelerometer& getAccerelometer() { return accerelometer; }

	void drawNodeId(int floatPos = 20);
	void drawNodeName(int floatPos = 20);

	// utils
	inline void beginTransform() const { transformGL(); }
	inline void endTransform() const { restoreTransformGL(); }

private:
	
	string name;
	ramAccelerometer accerelometer;
	
};

#pragma mark - ramNodeArray

enum ramNodeArrayType
{
	RAM_NODEARRAY_TYPE_ACTOR     = 0,
	RAM_NODEARRAY_TYPE_RIGIDBODY = 1
};

class ramNodeArray : public ofxNodeArray::NodeArray<ramNodeArray, ramNode>
{
public:

	ramNodeArray();
	ramNodeArray(const ramNodeArray& copy) { *this = copy; }
	ramNodeArray& operator=(const ramNodeArray& copy);
	virtual ~ramNodeArray() {}

	inline bool isOutdated() const { return (ofGetElapsedTimef() -  last_update_client_time) > RAM_OUTDATED_DURATION; }
	inline float getTimestamp() const { return last_update_client_time; }

	inline void setType(ramNodeArrayType t) { type = t; }
	inline bool isActor() const { return type == RAM_NODEARRAY_TYPE_ACTOR; }
	inline bool isRigid() const { return type == RAM_NODEARRAY_TYPE_RIGIDBODY; }
	inline bool isTypeOf(ramNodeArrayType t) const { return type == t; }
	
	virtual void updateWithOscMessage(const ofxOscMessage &m);

protected:

	ramNodeArrayType type;

	float last_timestamp;
	float current_timestamp;
	float last_update_client_time;
};

#pragma mark - ramRigidBody

class ramRigidBody : public ramNodeArray
{

public:

	ramRigidBody();
	ramRigidBody(const ramNodeArray &copy) { *this = copy; }

	ramRigidBody& operator=(const ramNodeArray &copy);

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
	ramActor(const ramNodeArray &copy);
	virtual ~ramActor();

	ramActor& operator=(const ramNodeArray &copy);

	virtual void updateWithOscMessage(const ofxOscMessage &m);
	
	static string getJointName(int jointId) { return jointName[jointId]; }
	static vector<string> getJointNames();

private:
	static string jointName[NUM_JOINTS];
	void dispose();

	void setupTree();
};