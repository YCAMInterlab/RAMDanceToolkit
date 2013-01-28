#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "CircleFinder.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofImage img;
	CircleFinder circleFinder;
};
