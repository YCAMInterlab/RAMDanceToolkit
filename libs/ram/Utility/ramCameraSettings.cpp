#include "ramCameraSettings.h"

void ramCameraSettings::prepareForPlay()
{
	bMoving = true;
	moving_from = pos;
	moving_start_time = ofGetElapsedTimef();
	moving_end_time = moving_start_time + moving_duration;
	moving_deg = 0.0f;
}
void ramCameraSettings::reset()
{
	bMoving = false;
	pos = moving_from;
}



vector<string> ramCameraSettings::getCamNames(ofxXmlSettings& setting)
{
	const int numCams = setting.getNumTags("cam");
	
	vector<string> names;
	names.clear();
	names.resize(numCams);
	
	for (int i=0; i<numCams; i++)
	{
		setting.pushTag("cam", i);
		{
			names.at(i) = setting.getValue("name", "");
		}
		setting.popTag();
	}
	return names;
}

vector<ofVec3f> ramCameraSettings::getCamPositions(ofxXmlSettings& setting)
{
	const int numCams = setting.getNumTags("cam");
	
	vector<ofVec3f> positions;
	positions.clear();
	positions.resize(numCams);
	
	for (int i=0; i<numCams; i++)
	{
		setting.pushTag("cam", i);
		{
			positions.at(i) = ofVec3f(setting.getValue("x", 0),
									  setting.getValue("y", 0),
									  setting.getValue("z", 0));
		}
		setting.popTag();
	}
	
	if (numCams==0) positions.at(0) = ofVec3f(0,0,0);
	return positions;
}

vector<ramCameraSettings> ramCameraSettings::getSettings(ofxXmlSettings& setting)
{
	const int numCams = setting.getNumTags("cam");
	
	vector<ramCameraSettings> settings;
	settings.clear();
	
	for (int i=0; i<numCams; i++)
	{
		setting.pushTag("cam", i);
		{
			ramCameraSettings s = ramCameraSettings(setting);
			
			if ( setting.pushTag("movement") )
			{
				
				s.moving_type = setting.getValue("type", "") == "circle" ? MOVING_CIRCLE : MOVING_LINEAR;
				
				s.look_at = ofVec3f(setting.getValue("look_at:x", 0.0),
										   setting.getValue("look_at:y", 0.0),
										   setting.getValue("look_at:z", 0.0));
				
				// linear
				s.moving_duration = setting.getValue("duration", 3000.0);
				if( s.moving_duration < 0 ) s.moving_duration = 3000.0;
				
				s.moving_to = ofVec3f(setting.getValue("move_to:x", 0.0),
									  setting.getValue("move_to:y", 300.0),
									  setting.getValue("move_to:z", 600.0));
				
				// circle
				s.moving_speed = setting.getValue("speed", 10.0);
				
				s.moving_radius = setting.getValue("radius", 300.0);
				
				s.moving_axis_pos = ofVec3f(setting.getValue("axis_pos:x", 100.0),
											setting.getValue("axis_pos:y", 100.0),
											setting.getValue("axis_pos:z", 0.0));
				
				s.moving_axis_up_vector = ofVec3f(setting.getValue("axis_up_vector:x", 1.0),
												  setting.getValue("axis_up_vector:y", 0.0),
												  setting.getValue("axis_up_vector:z", 0.0));
				
				setting.popTag();
			}
			
			settings.push_back(s);
		}
		setting.popTag();
	}
	return settings;
}