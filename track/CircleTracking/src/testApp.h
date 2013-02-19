#pragma once

#include "ofMain.h"
#include "CircleSensor.h"
#include "ofxUI.h"
#include "ofxOscSender.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam easyCam;
	vector<ofPtr<CircleSensor> > sensors;
	ofxUICanvas *gui;
    
    ofVec3f combined, filtered;
    
    ofxOscSender osc;
};

