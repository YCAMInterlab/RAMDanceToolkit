#pragma once

#include "ofxCv.h"
#include "ofxKinect.h"
#include "CircleFinder.h"

class CircleSensor {
public:
	CircleFinder circleFinder;
	ofxKinect kinect;
	ofMesh cloud;
	ofImage valid, background;
	vector<ofVec3f> trackedPositions;
	vector<ofVec3f> registrationSamples;
	ofMatrix4x4 registration;
	
	bool backgroundClear;
	bool registrationClear;
	bool backgroundCalibrate;
	unsigned char backgroundThreshold;
	float sampleRadius;
	
	CircleSensor();
	void setup();
	void update();
	void drawCloud();
	void drawDebug();
	void sampleRegistration();
	void updateRegistration(CircleSensor& reference);
	bool oneTrackedPosition();
	~CircleSensor();
};
