#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "CircleFinder.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	CircleFinder circleFinder;
	ofxKinect kinect;
	ofMesh cloud;
	ofEasyCam easyCam;
};

