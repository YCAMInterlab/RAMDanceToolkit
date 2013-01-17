#pragma once

#include "ofMain.h"
#include "CircleSensor.h"
#include "ofxAutoControlPanel.h"
#include "ofxTiming.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam easyCam;
	ofxAutoControlPanel gui;
	vector<ofPtr<CircleSensor> > sensors;
	DelayTimer registrationCalibrationTimer;
};

