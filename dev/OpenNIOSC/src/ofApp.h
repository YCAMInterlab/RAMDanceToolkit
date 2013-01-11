#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void userEvent(ofxOpenNIUserEvent & event);
	
	ofxOpenNI openNIDevice;	
};