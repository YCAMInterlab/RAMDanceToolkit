#pragma once
#include "ofxAutoControlPanel.h"
#include "ofxControlPanel.h"
#include "ofxXmlSettings.h"


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
	
public:
	
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
	
	template<typename T>
	void addScenePanels(vector<T> scenes)
	{
		const int numScenes = scenes.size();
		
		addPanel( "All Scenes" );
		addToggle( "Draw Actor", true );
		for (int i=0; i<scenes.size(); i++)
		{
			string key = scenes.at(i)->getSceneEnableKey();
			addToggle( key, false );
		}
		for (int i=0; i<scenes.size(); i++)
		{
			scenes.at(i)->setup();
			scenes.at(i)->refreshControlPanel( *this );
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
};