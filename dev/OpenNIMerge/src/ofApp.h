#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class Joint {
public:
	ofVec3f position;
	float confidence;
	
	Joint()
	:confidence(0) {
	}
};

class User {
public:
	bool isFound, isTracking, isSkeleton, isCalibrating;
	map<int, Joint> joints;
	
	User()
	:isFound(false)
	,isTracking(false)
	,isSkeleton(false)
	,isCalibrating(false) {
	}
};

class OpenNI {
public:
	map<int, User> users;
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxOscReceiver osc;
	map<int, OpenNI> opennis;
	ofEasyCam cam;
};