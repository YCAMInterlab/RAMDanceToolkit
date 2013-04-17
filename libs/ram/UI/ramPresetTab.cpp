// 
// ramPresetTab.cpp - RAMDanceToolkit
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

#include "ramPresetTab.h"

#include "ramCameraManager.h"

ramPresetTab::ramPresetTab()
:ofxUITab("Presets", false)
,preset_cam_index(1)
,use_node_cam(false)
,cam_position(false)
,cam_look_at(false)
{
	// should probably be a list of named presets instead of a grid
	matrix = addToggleMatrix("Presets", 4, 4);
	matrix->setAllowMultiple(false);
	addSpacer();
	
	vector<string> cameraPresetNames;
	cameraPresetNames.push_back("Low");
	cameraPresetNames.push_back("High");
	cameraPresetNames.push_back("Overhead");
	preset_cam_radio = addRadio("Camera position", cameraPresetNames);
	preset_cam_radio->getToggles()[preset_cam_index]->setValue(true);
	addSpacer();
	
    // nodecam
    addLabel("NodeCam", OFX_UI_FONT_LARGE);
    addLabel("1. Click a node on actor", OFX_UI_FONT_SMALL);
    addLabel("which you want to set as", OFX_UI_FONT_SMALL);
    addLabel("camera pos or lookAt.", OFX_UI_FONT_SMALL);
    
    addLabel("2. Click a button below.", OFX_UI_FONT_SMALL);
    addLabel("a joint with red circle", OFX_UI_FONT_SMALL);
    addLabel("will be set as pos or lookAt.", OFX_UI_FONT_SMALL);
    addButton("Set Camera Position", &cam_position, 15, 15);
    addButton("Set Camera LookAt", &cam_look_at, 15, 15);
    
    addLabel("3. Enable NodeCam.", OFX_UI_FONT_SMALL);
    addToggle("Enable Node Cam", &use_node_cam, 30, 30);
    
	autoSizeToFitWidgets();
	
	ofAddListener(ofEvents().update, this, &ramPresetTab::update);
	ofAddListener(ofEvents().draw, this, &ramPresetTab::draw);
	ofAddListener(newGUIEvent, this, &ramPresetTab::guiEvent);
}

void ramPresetTab::update(ofEventArgs& e)
{
    // why 10 frames?
	if(ofGetFrameNum() < 10)
    {
		preset_cam_radio->getToggles()[preset_cam_index]->setValue(true);
		preset_cam_radio->getToggles()[preset_cam_index]->triggerSelf();
	}
    
    
    // nodecam
    if (cam_position)
    {
        nf_pos = ramActorManager::instance().getLastSelectedNodeIdentifer();
        nf_pos.findOne(node_pos);
    }
    if (cam_look_at)
    {
        nf_look_at = ramActorManager::instance().getLastSelectedNodeIdentifer();
        nf_look_at.findOne(node_look_at);
    }
    
    if (use_node_cam)
    {
        ofVec3f pos = node_pos.getGlobalPosition();
        ofVec3f lookAt = node_look_at.getGlobalPosition();
        
        ramCameraManager::instance().getActiveCamera().setGlobalPosition(pos);
        ramCameraManager::instance().getActiveCamera().lookAt(lookAt);
    }
}

void ramPresetTab::draw(ofEventArgs& e)
{
    if (use_node_cam) return;
    
    ramBeginCamera();
    glEnable(GL_DEPTH_TEST);
    if (nf_pos.found())
    {
        ofVec3f pos = node_pos.getGlobalPosition();
        pos.y += 20;
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofLine(node_pos, pos);
        ofDrawBitmapString("[Camera]", pos);
        ofPopStyle();
    }

    if (nf_look_at.found())
    {
        ofVec3f pos = node_look_at.getGlobalPosition();
        pos.y += 20;
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofLine(node_look_at, pos);
        ofDrawBitmapString("[LookAt]", pos);
        ofPopStyle();
    }
    glDisable(GL_DEPTH_TEST);
    ramEndCamera();
}


void ramPresetTab::guiEvent(ofxUIEventArgs &e)
{
	int choice = getChoice(e, preset_cam_radio);
	if(choice != -1)
    {
		int indices[] = {0, 1, 5};
		int choice = getChoice(preset_cam_radio);
		ramCameraManager::instance().rollbackDefaultCameraSetting(indices[choice]);
	}
}