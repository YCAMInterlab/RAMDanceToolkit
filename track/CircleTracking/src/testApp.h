#pragma once

#include "ofMain.h"
#include "CircleSensor.h"
#include "ofxTiming.h"
#include "ofxUI.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam easyCam;
	vector<ofPtr<CircleSensor> > sensors;
	DelayTimer registrationCalibrationTimer;;
	
	ofxUICanvas *gui;
};

