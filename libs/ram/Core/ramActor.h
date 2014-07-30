// 
// ramActor.h - RAMDanceToolkit
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
	friend class ramActor;
	friend class ramRigidBody;

public:

	ramNode() : initialized(false) {}
	ramNode(int index) : ofxNodeArray::Node<ramNode>(index) {}
	
	ramNode(const ramNode& copy) { *this = copy; }
	ramNode& operator=(const ramNode& copy);
	
	inline void setName(const string n) { name = n; }
	const string& getName() const { return name; }
	
	const ofMatrix4x4& getTransformMatrix() const { return getLocalTransformMatrix(); }
	const ofMatrix4x4& getMatrix() const { return getLocalTransformMatrix(); }

	inline ofVec3f getVelocity() const { return accelerometer.velocity; }
	inline ofVec3f getAcceleration() const { return accelerometer.acceleration; }
	inline ofQuaternion getAngularVelocity() const { return accelerometer.angular_velocity; }
	inline ofQuaternion getAngularAcceleration() const { return accelerometer.angular_acceleration; }

	inline ramAccelerometer& getAccelerometer() { return accelerometer; }

	void drawNodeId(int floatPos = 20) const;
	void drawNodeName(int floatPos = 20) const;

	// utils
	inline void beginTransform() const { transformGL(); }
	inline void endTransform() const { restoreTransformGL(); }

	
	bool initialized;
	
private:
	
	string name;
	ramAccelerometer accelerometer;
	
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
	inline void setTimestamp(const float t) { last_update_client_time = t; }

	inline void setType(ramNodeArrayType t) { type = t; }
	inline bool isActor() const { return type == RAM_NODEARRAY_TYPE_ACTOR; }
	inline bool isRigid() const { return type == RAM_NODEARRAY_TYPE_RIGIDBODY; }
	inline bool isTypeOf(ramNodeArrayType t) const { return type == t; }
    
	inline void setPlayback(bool b) { is_playback = b; }
	inline bool isPlayback() const { return is_playback; }
    
	virtual void updateWithOscMessage(const ofxOscMessage &m);
    
    ofPoint getCentroid() const;
    
protected:

	ramNodeArrayType type;
    bool is_playback;

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