#pragma once
#include "ramBaseScene.h"
#include "ofxXmlSettings.h"

/*!
 XML Example
 
 
 // Linear example
 <cam>
	 <name>Moving 2</name>
	 <x>0</x><y>300</y><z>600</z> // initial pos
	 <movement>
		 <type>linear</type> // dependencies: <move_to>, <duration>, <look_at>
		 <duration>1</duration>
		 <look_at>
			 <x>0</x>
			 <y>150</y>
			 <z>0</z>
		 </look_at>
		 <move_to> // distination
			 <x>-450</x>
			 <y>50</y>
			 <z>450</z>
		 </move_to>
	 </movement>
 </cam>
 
 
 // Circle example
 <cam>
	 <name>Moving 2</name>
	 <x>0</x><y>300</y><z>600</z> // initial pos
	 <movement>
		 <type>circle</type> // dependencies: <radius>, <speed>, <axis_pos>, <axis_up_vector>, <look_at>
		 <radius>100.0</radius>
		 <speed>1.0</speed>
		 <look_at>
			 <x>100</x>
			 <y>150</y>
			 <z>0</z>
		 </look_at>
		 <axis_pos> // linear or circle
			 <x>300.0</x>
			 <y>100.0</y>
			 <z>0.0</z>
		 </axis_pos>
		 <axis_up_vector>
			 <x>0.0</x>
			 <y>0.0</y>
			 <z>1.0</z>
		 </axis_up_vector>
	 </movement>
 </cam>
*/


class CameraMover : public ramBaseScene
{
	
private:
	ofxXmlSettings mSettingXml;
	vector<ramCameraSettings> mSettings;
	ramCameraSettings mCurSetting;
	bool bCameraOn;
	
	ofxUICanvas *mPanel;
	string mXmlPath;
	
public:
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		mPanel = panel;
		refreshPanel(panel);
		
		ofAddListener(panel->newGUIEvent, this, &CameraMover::onTriggered);
	}
	
	
	void setup(string path)
	{
		mXmlPath = path;
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &CameraMover::keyPressed);
	}
	
	
	void setup()
	{
		mXmlPath = ramToResourcePath("Settings/cam.moving.xml");
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &CameraMover::keyPressed);
	}
	
	void draw()
	{
		if (!mCurSetting.bMoving) return;
		
		// set new moving state
		mCurSetting.bMoving = (mCurSetting.moving_end_time >= ofGetElapsedTimef());
		
		float progress = (ofGetElapsedTimef()-mCurSetting.moving_start_time) / (mCurSetting.moving_duration);
		
		ramBeginCamera();
		
		// linear movement
		if ( mCurSetting.moving_type == ramCameraSettings::MOVING_LINEAR )
		{
			ofVec3f &from = mCurSetting.moving_from;
			ofVec3f &to = mCurSetting.moving_to;
			ofVec3f &lookAt = mCurSetting.look_at;
			ofVec3f newPos = from + progress * (to - from);
			
			
			if( bCameraOn )
			{
				getActiveCamera().setPosition( newPos );
				getActiveCamera().lookAt( lookAt );
			}
			else
			{
				ofSetColor(0);
				ofBox(newPos, 10);
				
				ofSetColor(255,0,0);
				ofBox(from, 10);
				ofBox(to, 10);
				ofDrawArrow(newPos, lookAt, 10);
			}
		}
		// circle movement
		else
		{
			ofVec3f &axis = mCurSetting.moving_axis_pos;
			ofVec3f &up_vector = mCurSetting.moving_axis_up_vector;
			ofVec3f &look_at = mCurSetting.look_at;
			float& radius = mCurSetting.moving_radius;
			float& speed = mCurSetting.moving_speed;
			float& rot = mCurSetting.moving_deg;
			
			rot += speed;
			
			ofNode axisNode;
			ofNode satelliteNode;
			
			axisNode.setPosition(axis);
			satelliteNode.setParent(axisNode);
			satelliteNode.setPosition(radius, 0.0f, 0.0f);
			
			ofQuaternion q(rot, up_vector);
			axisNode.setOrientation(q);
			
			if( bCameraOn )
			{
				getActiveCamera().setPosition( satelliteNode.getGlobalPosition() );
				getActiveCamera().lookAt( look_at );
			}
			else
			{
				ramBeginCamera();
				
				axisNode.draw();
				satelliteNode.draw();
				
				ofPushStyle();
				{
					// axis
					ofSetColor(0);
					ofLine(up_vector.getNormalized()* -1000 + axis, up_vector.getNormalized()* 1000 + axis);
					ofLine(axisNode.getGlobalPosition(), satelliteNode.getGlobalPosition());
					
					// camera
					ofSetColor(255,0,0);
					ofDrawArrow(satelliteNode.getGlobalPosition(), look_at, 10);
				}
				ofPopStyle();
				
				ramEndCamera();
			}
		}
		
		ramEndCamera();
	}
	
	void keyPressed(ofKeyEventArgs &e)
	{
		if (e.key == 'r')
			reloadSettings();
	}
	
	void reloadSettings()
	{
		mSettingXml.loadFile(mXmlPath);
		mSettings = ramCameraSettings::loadSettings(mSettingXml);
//		refreshPanel(mPanel);
		cout << "[" << getName() << "] settings reloaded." << endl;
	}
	
	void refreshPanel(ofxUICanvas *panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->removeWidgets();
		panel->resetPlacer();
		
		/// Label: Title
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Reload setting file", false, 50, 50);
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUILabel("or press 'r' key to reload xml settings", OFX_UI_FONT_SMALL));
		panel->addSpacer(gui.kLength, 2);
		
		
		/// Toggle: Camera on/off
		panel->addToggle("Camera On", &bCameraOn, 50, 50);
		panel->addSpacer(gui.kLength, 2);
		
		
		/// Radio: start moving
		vector<string> camNames;
		for (int i = 0; i < mSettings.size(); i++)
		{
			camNames.push_back(mSettings[i].name);
		}
		panel->addRadio("Camera Preset", camNames, OFX_UI_ORIENTATION_VERTICAL, gui.kDim, gui.kDim);
	}
	
	void onTriggered(ofxUIEventArgs &e)
	{
		string name = e.widget->getName();
		
		if (name == "Reload setting file")
		{
			reloadSettings();
		}
		else
		{
			for (int i=0; i<mSettings.size(); i++)
			{
				ramCameraSettings &setting = mSettings.at(i);
				
				if(name == setting.name)
				{
					setting.prepareForPlay();
					mCurSetting = setting;
					
					cout << "[" << getName() << "] current setting:" << mCurSetting.name << endl;
				}
				else
				{
					setting.reset();
				}
			}
		}
	}
	
	const string getName() { return "Camera Mover"; }
};

