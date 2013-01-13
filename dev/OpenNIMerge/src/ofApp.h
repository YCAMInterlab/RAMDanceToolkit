#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxCv.h"

inline bool checkAddress(ofxOscMessage& msg, string name, int position) {
	vector<string> address = ofSplitString(msg.getAddress(), "/", true);
	return address.size() > position && address[position] == name;
}

inline string getAddress(ofxOscMessage& msg, int position) {
	vector<string> address = ofSplitString(msg.getAddress(), "/", true);
	return address[position];
}

enum Joint {
    // start at root joint
    JOINT_TORSO = 0,
    JOINT_NECK,
    JOINT_HEAD,

    // left arm + shoulder
    JOINT_LEFT_SHOULDER,
    JOINT_LEFT_ELBOW,
    JOINT_LEFT_HAND,

    // right arm + shoulder
    JOINT_RIGHT_SHOULDER,
    JOINT_RIGHT_ELBOW,
    JOINT_RIGHT_HAND,

    // left leg
    JOINT_LEFT_HIP,
    JOINT_LEFT_KNEE,
    JOINT_LEFT_FOOT,

    // right leg
    JOINT_RIGHT_HIP,
    JOINT_RIGHT_KNEE,
    JOINT_RIGHT_FOOT,

    JOINT_COUNT,
    JOINT_UNKOWN
};

const static int skeletonLimbSize = 14;
const static int skeletonLimbs[] = {
	JOINT_HEAD, JOINT_NECK,
	JOINT_NECK, JOINT_TORSO,
	JOINT_RIGHT_SHOULDER, JOINT_NECK,
	JOINT_RIGHT_ELBOW, JOINT_RIGHT_SHOULDER,
	JOINT_RIGHT_HAND, JOINT_RIGHT_ELBOW,
	JOINT_LEFT_SHOULDER, JOINT_NECK,
	JOINT_LEFT_ELBOW, JOINT_LEFT_SHOULDER,
	JOINT_LEFT_HAND, JOINT_LEFT_ELBOW,
	JOINT_LEFT_HIP, JOINT_TORSO,
	JOINT_LEFT_KNEE, JOINT_LEFT_HIP,
	JOINT_LEFT_FOOT, JOINT_LEFT_KNEE,
	JOINT_RIGHT_HIP, JOINT_TORSO,
	JOINT_RIGHT_KNEE, JOINT_RIGHT_HIP,
	JOINT_RIGHT_FOOT, JOINT_RIGHT_KNEE,
};

class OscJoint {
public:
	ofVec3f position;
	float confidence;
	
	OscJoint()
	:confidence(0) {
	}
};

class OscUser {
public:
	bool isFound, isTracking, isSkeleton, isCalibrating;
	map<int, OscJoint> joints;
	
	OscUser()
	:isFound(false)
	,isTracking(false)
	,isSkeleton(false)
	,isCalibrating(false) {
	}
	void drawSkeleton() {
		for(int i = 0; i < skeletonLimbSize; i++) {
			int root = skeletonLimbs[i * 2];
			int parent = skeletonLimbs[i * 2 + 1];
			ofLine(joints[root].position, joints[parent].position);
		}
	}
	void update(ofxOscMessage& msg) {
		if(checkAddress(msg, "joints", 4)) {
			const int elementsPerOscJoint = 4;
			int numOscJoints = msg.getNumArgs() / elementsPerOscJoint;
			int j = 0;
			for(int i = 0; i < numOscJoints; i++) {
				OscJoint& joint = joints[i];
				joint.position.x = msg.getArgAsFloat(j++);
				joint.position.y = msg.getArgAsFloat(j++);
				joint.position.z = msg.getArgAsFloat(j++);
				joint.confidence = msg.getArgAsFloat(j++);
			}
		} else {
			isFound = msg.getArgAsInt32(0);
			isTracking = msg.getArgAsInt32(1);
			isSkeleton = msg.getArgAsInt32(2);
			isCalibrating = msg.getArgAsInt32(3);
		}
	}
};

class OscOpenNI {
public:
	static bool calibrating;
	map<int, OscUser> users;
	vector<ofVec3f> recentData;
	
	void update(ofxOscMessage& msg) {
		if(checkAddress(msg, "user", 2)) {
			int xnId = ofToInt(getAddress(msg, 3));
			users[xnId].update(msg);
			if(calibrating) {
				recentData.push_back(users[xnId].joints[JOINT_NECK].position);
			}
		}
	}
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofxOscReceiver osc;
	map<int, OscOpenNI> opennis;
	ofEasyCam cam;
	
	map<int, ofMatrix4x4> registration;
};