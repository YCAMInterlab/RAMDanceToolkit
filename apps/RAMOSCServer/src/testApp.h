//
//  testApp.h
//  RAMDanceToolkit
//
//  Created by motoishmz on 4/27/13.
//  Copyright (c) 2013 YCAMInterlab. All rights reserved.

#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOscSender.h"
#include "ramOscPlaybacker.h"

class testApp : public ofBaseApp
{
public:

	void setup();
	void update();
	void draw();
	void exit();
	
	void load(const string path);
	void setSpeed(const float new_speed);
	
	void dragEvent(ofDragInfo dragInfo);
	void onValueChanged(ofxUIEventArgs &e);
	
	
	vector<ramOscPlaybacker> playbackers;
	ofxOscSender sender;
	
	ofxUICanvas *GUI;
	ofxUITextInput *input_ip;
	ofxUITextInput *input_port;
	
	bool paused;
	bool reset_playhead;
	float speed;
};