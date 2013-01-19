#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void userEvent(ofxOpenNIUserEvent & event);
	
	ofxOpenNI openNI;
	ofxOscSender osc;
};