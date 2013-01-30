#pragma once

#include "ofxAutoControlPanel.h"
#include "ofxControlPanel.h"
#include "ofxXmlSettings.h"

#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramSceneBase.h"

class ramSceneBase;

class ramCameraSettings
{
public:
	
	enum MovingType
	{
		MOVING_LINEAR = 0,
		MOVING_CIRCLE = 1
	};
	
	string name;
	ofVec3f pos;
	ofVec3f lookAt;
	
	unsigned int moving_type;
	ofVec3f moving_look_at;
	bool bMoving;
	
	// linear moving
	ofVec3f moving_from;
	ofVec3f moving_to;
	float moving_duration;
	float moving_start_time;
	float moving_end_time;
	
	// circle moving
	ofVec3f moving_axis_pos;
	ofVec3f moving_axis_up_vector;
	float moving_radius;
	float moving_speed;
	float moving_deg;
	
	
	ramCameraSettings(){}
	ramCameraSettings(ofxXmlSettings& setting)
	{
		name = setting.getValue("name", "unknown");
		
		pos = ofVec3f(setting.getValue("pos:x", 0),
					  setting.getValue("pos:y", 0),
					  setting.getValue("pos:z", 0));
		
		lookAt = ofVec3f(setting.getValue("look_at:x", 0),
						 setting.getValue("look_at:y", 100),
						 setting.getValue("look_at:z", 0));
	}
	
	static vector<ramCameraSettings> getSettings(ofxXmlSettings& setting)
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
					
					s.moving_look_at = ofVec3f(setting.getValue("look_at:x", 0.0),
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
	
	
	static vector<string> getCamNames(ofxXmlSettings& setting)
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
	
	static vector<ofVec3f> getCamPositions(ofxXmlSettings& setting)
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
};

class ramControlPanel : public ofxAutoControlPanel
{

private:
	vector<ramCameraSettings> camSettings;
	bool bCheckCameras, bCheckScenes;
	
	string key_background,
	key_floor_pattern,
	key_floor_size,
	key_grid_size,
	key_camera_position;
	
	static ramControlPanel *_instance;
	
public:
	
	static ramControlPanel& instance();
	
	void setup()
	{
		ofxControlPanel::setTextColor( simpleColor(255, 255, 255, 100) );
		ofxControlPanel::setBackgroundColor( simpleColor(0, 0, 0, 90) );
		ofxAutoControlPanel::setup( 400, ofGetHeight() );
		
		/*!
			Panel: Config
		 */
		addPanel( "Config" );
		
		
		/* Background Color */
		addSlider( key_background, 0, 0, 255 );
		
		
		/* Default Floor Patterns */
		addMultiToggle( key_floor_pattern, 0, ramFloor::getFloorNames() );
		addSlider( key_floor_size, 600.0, 100.0, 1000.0 );
		addSlider( key_grid_size, 50.0, 10.0, 100.0 );
		
		
		/* Event Listeners */
		ofAddListener( ofEvents().update, this, &ramControlPanel::update );
		ofAddListener( ofEvents().draw, this, &ramControlPanel::draw );
		ofAddListener( ofEvents().exit, this, &ramControlPanel::exit );
	}
	
	void update( ofEventArgs &e )
	{
		
		/* GUI: camera */
		if ( bCheckCameras && hasValueChanged(key_camera_position) )
		{
			/* camera */
			int posIndex = getValueI( key_camera_position );
			ofVec3f pos = camSettings.at( posIndex ).pos;
			ramCameraManager::instance().getActiveCamera().setPosition( pos );
			ramCameraManager::instance().getActiveCamera().lookAt( ofVec3f(0,170,0) );
		}
		
		/* GUI: background */
		if ( hasValueChanged(key_background) )
		{
			float bgcolor = getValueF( key_background );
			ofBackground(bgcolor);
		}
	}
	
	void draw( ofEventArgs &e )
	{
		
	}
	
	void exit( ofEventArgs &e )
	{
		ofRemoveListener( ofEvents().update, this, &ramControlPanel::update );
		ofRemoveListener( ofEvents().draw, this, &ramControlPanel::draw );
		ofRemoveListener( ofEvents().exit, this, &ramControlPanel::exit );
	}
	
#pragma mark -
	
	
	/*!
		GUI Helpers
	 */
	void loadCameraSettings( string filename )
	{
		ofxXmlSettings xml_cam( filename );
		camSettings = ramCameraSettings::getSettings( xml_cam );
		addMultiToggle( key_camera_position, 0, ramCameraSettings::getCamNames(xml_cam) );
		
		bCheckCameras = true;
	}
	
	void addScenePanels(vector<ramSceneBase*>& scenes)
	{
		addPanel( "All Scenes" );
		addToggle( "Draw Actor", true );
		
		for (int i=0; i<scenes.size(); i++)
		{
			string key = scenes.at(i)->getSceneEnableKey();
			addToggle( key, false );
		}
		for (int i=0; i<scenes.size(); i++)
		{
			ramSceneBase *scene = scenes.at(i);
			scene->guiPtr = this;
			scene->setup();
			scene->setupControlPanel( *this );
		}
		
		bCheckScenes = true;
	}
	
	
#pragma mark -
	
	/*!
		ofxControlPanel
	 */
	bool hasValueChanged( string xmlName )
	{
		bool bChanged = ofxControlPanel::hasValueChanged(xmlName);
		if ( bChanged ) clearAllChanged();
		return bChanged;
	}
	
	bool hasValueChangedInPanel( string whichPanel )
	{
		bool bChanged = ofxControlPanel::hasValueChangedInPanel( whichPanel );
		if ( bChanged ) clearAllChanged();
		return bChanged;
	}
	
	/*!
		ofxAutoControlPanel
	 */
	// usage: panel.hasValueChanged(variadic(1)(2)(3)(4)(5));
	bool hasValueChanged( const vector<string>& values )
	{
		bool bChanged = ofxAutoControlPanel::hasValueChanged( values );
		if ( bChanged ) clearAllChanged();
		return bChanged;
	}
	
	
	// --
	// usage: panel.addTextDropDown("DropDownName", 0, variadic(1)(2)(3)(4)(5));
	guiTypeTextDropDown * addTextDropDown( string name, int defaultBox, const vector<string>& names )
	{
		return ofxControlPanel::addTextDropDown( name, name, defaultBox, names );
	}
	
protected:
	
	ramControlPanel()
	{
		bCheckCameras = false;
		bCheckScenes = false;
		
		key_background = "Background";
		key_floor_pattern = "Floor pattern";
		key_floor_size = "Floor size";
		key_grid_size = "Grid size";
		key_camera_position = "Camera Position";
	}

};

inline ramControlPanel& ramGetGUI()
{
	return ramControlPanel::instance();
}
