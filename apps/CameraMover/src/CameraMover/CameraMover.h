#pragma once
#include "ramSceneBase.h"
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


class CameraMover : public ramSceneBase
{
	
private:
	ofxXmlSettings settings_xml;
	vector<ramCameraSettings> settings;
	bool bCameraOn;
	
public:
	
	CameraMover()
	{
		setSceneName("Camera Mover");
		
		// ---
		
		bCameraOn = false;
		reloadSettings();
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		
		gui.addPanel( getSceneEnableKey() );
		gui.addToggle("Camera On");
		gui.addMultiToggle( "Moving", 0, ramCameraSettings::getCamNames(settings_xml) );
		gui.clearAllChanged();
	}
	
	void setup()
	{
		ofAddListener(ofEvents().keyPressed, this, &CameraMover::keyPressed);
	}
	
	void draw()
	{
		if (guiPtr->hasValueChanged( "Camera On" ))
		{
			bCameraOn = guiPtr->getValueB( "Camera On" );
		}
		
		// moving state update
		if (guiPtr->hasValueChanged("Moving"))
		{
			int selectedIndex = guiPtr->getValueI("Moving");
			
			for (int i=0; i<settings.size(); i++)
			{
				ramCameraSettings &set = settings.at(i);
				
				if (selectedIndex == i)
				{
					set.bMoving = true;
					set.moving_from = set.pos;
					set.moving_start_time = ofGetElapsedTimef();
					set.moving_end_time = set.moving_start_time + set.moving_duration;
					set.moving_deg = 0.0f;
				}
				else
				{
					set.bMoving = false;
					set.pos = set.moving_from;
				}
			}
		}
		
		// cam position update
		for (int i=0; i<settings.size(); i++)
		{
			ramCameraSettings& set = settings.at(i);
			
			// ignore non-moving cam
			if ( !set.bMoving ) continue;
			
			// set new moving state
			set.bMoving = (set.moving_end_time >= ofGetElapsedTimef());
			
			// update cam pos if needed
			if ( set.bMoving )
			{
				float progress = (ofGetElapsedTimef()-set.moving_start_time) / (set.moving_duration);
				
				ramCameraBegin();
				
				// linear movement
				if ( set.moving_type == ramCameraSettings::MOVING_LINEAR )
				{
					ofVec3f &from = set.moving_from;
					ofVec3f &to = set.moving_to;
					ofVec3f newPos = from + progress * (to - from);
					
					if( bCameraOn )
					{
						getActiveCamera().setPosition( newPos );
						getActiveCamera().lookAt( set.moving_look_at );
					}
					else
					{
						ofSetColor(0);
						ofBox(newPos, 10);
						
						ofSetColor(255,0,0);
						ofBox(from, 10);
						ofBox(to, 10);
					}
				}
				// circle movement
				else
				{
					ofVec3f &axis = set.moving_axis_pos;
					ofVec3f &up_vector = set.moving_axis_up_vector;
					ofVec3f &look_at = set.moving_look_at;
					float& radius = set.moving_radius;
					float& speed = set.moving_speed;
					float& rot = set.moving_deg;
					
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
						ramCameraBegin();
						
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
						
						ramCameraEnd();
					}
				}
				
				ramCameraEnd();
			}
		}
	}
	
	void keyPressed(ofKeyEventArgs &e)
	{
		if (e.key == 'r')
			reloadSettings();
	}
	
	void reloadSettings()
	{
		settings_xml.loadFile("settings.camera.xml");
		settings = ramCameraSettings::getSettings(settings_xml);
	}
};

