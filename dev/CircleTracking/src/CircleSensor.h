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
	
	bool backgroundClear;
	bool backgroundCalibrate;
	unsigned char backgroundThreshold;
	float sampleRadius;
	
	CircleSensor();
	void setup();
	void update();
	void draw();
	void drawDebug();
	~CircleSensor();
};