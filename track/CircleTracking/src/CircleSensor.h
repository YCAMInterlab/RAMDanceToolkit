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
	
    float deadZoneRadius;
    vector<ofVec2f> deadZones;
    
    vector<ofVec2f> centers;
    vector<float> radii;
    vector<ofVec3f> trackedPositions;
    
	vector<ofVec3f> registrationSamples, referenceSamples;
	ofMatrix4x4 registration;
	
	bool backgroundClear;
	bool registrationClear;
	bool backgroundCalibrate;
	unsigned char backgroundThreshold;
	float sampleRadius;
    int minSamples;
	
	CircleSensor();
	void setup();
	void update();
	void drawCloud();
	void drawDebug();
	void updateRegistration(ofVec3f& reference, float accuracy = .99);
	bool oneTrackedPosition();
    void setDeadZones();
	~CircleSensor();
};
