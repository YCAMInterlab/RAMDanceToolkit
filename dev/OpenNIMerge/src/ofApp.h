#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

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
};

class OscOpenNI {
public:
	map<int, OscUser> users;
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxOscReceiver osc;
	map<int, OscOpenNI> opennis;
	ofEasyCam cam;
};