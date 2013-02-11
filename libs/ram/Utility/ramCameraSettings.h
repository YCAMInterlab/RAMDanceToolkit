#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

class ramCameraSettings
{
	
public:
	
	ramCameraSettings(){}
	ramCameraSettings(ofxXmlSettings& setting)
	{
		name = setting.getValue("name", "unknown");
		
		pos = ofVec3f(setting.getValue("pos:x", 0),
					  setting.getValue("pos:y", 0),
					  setting.getValue("pos:z", 0));
		
		look_at = ofVec3f(setting.getValue("look_at:x", 0),
								 setting.getValue("look_at:y", 100),
								 setting.getValue("look_at:z", 0));
	}
	
	enum MovingType
	{
		MOVING_LINEAR = 0,
		MOVING_CIRCLE = 1
	};
	
	string name;
	ofVec3f pos;
	ofVec3f look_at;
	
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
	
	
	static vector<string> getCamNames(ofxXmlSettings& setting);
	static vector<ofVec3f> getCamPositions(ofxXmlSettings& setting);
	static vector<ramCameraSettings> getSettings(ofxXmlSettings& setting);
	static ramCameraSettings getSettingsAt(ofxXmlSettings& setting);
};