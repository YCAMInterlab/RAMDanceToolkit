// 
// MovingCam.h - RAMDanceToolkit
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

#include "ofxXmlSettings.h"
#include "ramBaseScene.h"

class MovingCam : public rdtk::BaseScene
{
    
    enum MovingState
    {
        STATE_UNKNOWN = -1000,
        STATE_STOP = 0,
        STATE_MOVING_UP = 1,
        STATE_MOVING_DOWN = 2
    };
    
    struct CameraSetting
    {
        ofVec3f pos;
        ofVec3f lookAt;
        float fov;
        float angle;
    };
    
    CameraSetting base, opposite, camera, move_from, move_to;
    MovingState state;
    float moving_start_time;
    float moving_duration;
    float moving_radius;
	ofVec3f moving_axis;
    bool is_camera_enabled;
    inline float getElapsedTime() const { return ofGetElapsedTimef() - moving_start_time; }
	inline bool isCameraMoving() const { return state == STATE_MOVING_UP || state == STATE_MOVING_DOWN; }
    
public:
	
	void drawImGui()
	{
		ImGui::Text("Press [u] to move up");
		ImGui::Text("Press [d] to move down");
		ImGui::Separator();
		if (ImGui::Button("Start move up")) moveUp();
		if (ImGui::Button("Start move down")) moveDown();
		ImGui::Separator();
		if (ImGui::Button("Reset & Reload XML")) resetCamera();
		ImGui::Checkbox("Enable Camera", &is_camera_enabled);
	}
	
	void setupControlPanel()
	{
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
        
        panel->addLabel("Press [u] to move up", OFX_UI_FONT_SMALL);
        panel->addLabel("Press [d] to move down", OFX_UI_FONT_SMALL);
        panel->addSpacer();
        panel->addButton("Start move up", false);
        panel->addButton("Start move down", false);
        panel->addButton("Reset & Reload XML", false);
        panel->addSpacer();
        panel->addToggle("Enable Camera", &is_camera_enabled);
        
        ofAddListener(panel->newGUIEvent, this, &MovingCam::onValuChanged);
	}
	
	void setup()
	{
        is_camera_enabled = false;
        resetCamera();
	}
	
	void update()
	{
        if (isCameraMoving())
        {
            assert(state == STATE_MOVING_UP || state == STATE_MOVING_DOWN);
            
            const float direction =
                (state == STATE_MOVING_UP) ? 1 :
                (state == STATE_MOVING_DOWN) ? -1 : STATE_UNKNOWN;
            
            const float progress = getElapsedTime() / moving_duration;
            const float angle = (abs(move_from.angle)*2 * progress) * direction;
            
            camera.pos = move_from.pos.getRotated(angle, moving_axis);
			
            if (progress >= 1.0)
            {
                camera.pos = move_to.pos;
                resetCamera();
            }
        }
        
        if (is_camera_enabled)
        {
            getCameraManager().getActiveCamera().setPosition(camera.pos);
            getCameraManager().getActiveCamera().lookAt(camera.lookAt);
            getCameraManager().getActiveCamera().setFov(camera.fov);
        }
	}
	
	void draw()
	{
        if (is_camera_enabled)
            return;
        
		rdtk::BeginCamera();
        
        ofPushStyle();
        
        // from, to
        ofSetColor(255,0,0);
        ofDrawBox(base.pos, 10);
        ofDrawBox(opposite.pos, 10);
		
        ofVec3f strPos1 = move_from.pos;
        strPos1.y += 20;
        ofDrawBitmapString("From", strPos1);
        
        ofVec3f strPos2 = move_to.pos;
        strPos2.y += 20;
        ofDrawBitmapString("To", strPos2);
        
		// camera
		ofSetColor(0, 0, 255);
		ofVec3f curStrPos = camera.pos;
		curStrPos.y += 20;;
		ofDrawBitmapString("Camera", curStrPos);
		ofDrawBox(camera.pos, 10);
		
		// lookAt
		ofSetColor(0, 255, 0);
		ofVec3f lookAtStrPos = camera.lookAt;
		lookAtStrPos.y += 20;;
		ofDrawBitmapString("Look At", lookAtStrPos);
		ofDrawBox(camera.lookAt, 10);
		
		ofDrawLine(camera.lookAt, camera.pos);
		
        ofPopStyle();
        
        rdtk::EndCamera();
	}
    
    void reloadXML()
    {
		const string filePath = ofToDataPath("MovingCam.xml", true);
		
		if (!ofFile::doesFileExist(filePath))
		{
			ofxXmlSettings XML;
			XML.setValue("cam:pos:x", -620);
			XML.setValue("cam:pos:y", 300);
			XML.setValue("cam:pos:z", 620);
			XML.setValue("cam:look_at:x", 0);
			XML.setValue("cam:look_at:y", 50);
			XML.setValue("cam:look_at:z", 0);
			XML.setValue("cam:fov", 27);
			XML.setValue("cam:duration", 104);
			XML.saveFile(filePath);
		}
		
		ofxXmlSettings XML;
		XML.loadFile(filePath);
        
        base.pos.x = XML.getValue("cam:pos:x", 100);
        base.pos.y = XML.getValue("cam:pos:y", 100);
        base.pos.z = XML.getValue("cam:pos:z", 100);
        base.lookAt.x = XML.getValue("cam:look_at:x", 0);
        base.lookAt.y = XML.getValue("cam:look_at:y", 0);
        base.lookAt.z = XML.getValue("cam:look_at:z", 0);
        base.fov = XML.getValue("cam:fov", 60);
        
        moving_duration = XML.getValue("cam:duration", 100);
        moving_radius = base.pos.distance(ofVec3f::zero());
		moving_axis = base.pos.getPerpendicular(ofVec3f(0, 1, 0));
        const float triHeight = base.pos.y;
        const float triBase = sqrt(pow(moving_radius, 2) - pow(triHeight, 2));
        base.angle = ofRadToDeg( atan(triHeight/triBase) );

		
		// creating mirror node at Y-Axis opposite side
		opposite = base;
        opposite.pos.y *= -1;
        opposite.angle *= -1;
		
		
		// setting default position, fov, lookAt
		camera.pos = base.pos;
		camera.lookAt = base.lookAt;
		camera.fov = base.fov;
		move_from.pos = base.pos;
		move_to.pos = opposite.pos;
    }
    
    void resetCamera()
    {
        reloadXML();
        moving_start_time = ofGetElapsedTimef();
        state = STATE_STOP;
    }
    
    void moveUp()
    {
        resetCamera();
        state = STATE_MOVING_UP;
        move_from = (base.pos.y > 0) ? opposite : base;
        move_to = (base.pos.y > 0) ? base : opposite;
		camera = move_from;
    }
    
    void moveDown()
    {
        resetCamera();
        state = STATE_MOVING_DOWN;
        move_from = (base.pos.y > 0) ? base : opposite;
        move_to = (base.pos.y > 0) ? opposite : base;
		camera = move_from;
    }
    
    void onValuChanged(ofxUIEventArgs &e)
    {
        const string name = e.widget->getName();
        
        if (name == "Reset & Reload XML")
        {
            if (static_cast<ofxUIButton*>(e.widget)->getValue())
                resetCamera();
        }
        
        else if (name == "Start move up")
        {
            if (static_cast<ofxUIButton*>(e.widget)->getValue())
                moveUp();
        }
        
        else if (name == "Start move down")
        {
            if (static_cast<ofxUIButton*>(e.widget)->getValue())
                moveDown();
        }
    }
    
    void onKeyPressed(ofKeyEventArgs &e)
    {
        switch (e.key)
        {
            case 'u': moveUp(); break;
            case 'd': moveDown(); break;
            default: break;
        }
    }
    
    void onEnabled()
    {
		cout << "[Unit enabled] " << getName() << endl;
        ofAddListener(ofEvents().keyPressed, this, &MovingCam::onKeyPressed);
    }
	
	void onDisabled()
	{
		cout << "[Unit disabled] " << getName() << endl;
		getCameraManager().rollbackDefaultCameraSetting();
        ofRemoveListener(ofEvents().keyPressed, this, &MovingCam::onKeyPressed);
        
	}
	
	string getName() const { return "MovingCam"; }
};
