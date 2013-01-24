#pragma once

#include "ofMain.h"
#include "CircleSensor.h"
#include "ofxUI.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam easyCam;
	vector<ofPtr<CircleSensor> > sensors;
	
    ofVec3f filtered;
	ofxUICanvas *gui;
};

