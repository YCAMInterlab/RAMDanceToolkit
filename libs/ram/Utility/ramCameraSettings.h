// 
// ramCameraSettings.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

class ramCameraSettings
{

public:

	static vector<ramCameraSettings> loadSettings(ofxXmlSettings& setting);

	ramCameraSettings() {}
	ramCameraSettings(ofxXmlSettings& setting)
	{
		name = setting.getValue("name", "unknown");

		pos = ofVec3f(setting.getValue("pos:x", 0),
					  setting.getValue("pos:y", 0),
					  setting.getValue("pos:z", 0));

		look_at = ofVec3f(setting.getValue("look_at:x", 0),
						  setting.getValue("look_at:y", 100),
						  setting.getValue("look_at:z", 0));
		
		fov = setting.getValue("fov", 60.0);
	}

	enum MovingType
	{
		MOVING_LINEAR = 0,
		MOVING_CIRCLE = 1
	};

	string name;
	ofVec3f pos;
	ofVec3f look_at;
	float fov;

	unsigned int moving_type;
	bool bMoving;

	/// linear moving
	ofVec3f moving_from;
	ofVec3f moving_to;
	float moving_duration;
	float moving_start_time;
	float moving_end_time;

	/// circle moving
	ofVec3f moving_axis_pos;
	ofVec3f moving_axis_up_vector;
	float moving_radius;
	float moving_speed;
	float moving_deg;

	///
	void prepareForPlay();
	void reset();
};