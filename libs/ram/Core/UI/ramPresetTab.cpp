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
#include "ramSceneManager.h"

using namespace rdtk;

PresetTab::PresetTab()
:ofxUITab("Presets", false)
,preset_cam_index(1)
,use_node_cam(false)
,cam_position(false)
,cam_look_at(false)
{}

void PresetTab::setup(bool usePresetScenes)
{
	// show preset scenes button if this project is RAMDanceToolkit
	// bool usePresetScenes is passed from second arg of ramInitialize()
	if (usePresetScenes)
	{
		// should probably be a list of named presets instead of a grid
		addLabelButton("Line", false);
		addLabelButton("Hasty Chase", false);
		addLabelButton("HC + Stamp", false);
		addLabelButton("HC + Stamp + Natto", false);
		addLabelButton("HC + Future", false);
		addLabelButton("Line + Future", false);
		// ignore win32
#ifndef TARGET_WIN32
		addLabelButton("Particles + Future", false);
		addLabelButton("Particles", false);
#else
		addLabelButton("Future", false); // only for windows
#endif
		addSpacer();
	}
	
	vector<string> cameraPresetNames;
	cameraPresetNames.push_back("Low");
	cameraPresetNames.push_back("High");
	cameraPresetNames.push_back("Overhead");
	preset_cam_radio = addRadio("Camera position", cameraPresetNames);
	preset_cam_radio->getToggles()[preset_cam_index]->setValue(true);
	CameraManager::instance().rollbackDefaultCameraSetting(1);
	
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
	
	ofAddListener(ofEvents().update, this, &PresetTab::update);
	ofAddListener(ofEvents().draw, this, &PresetTab::draw);
	ofAddListener(newGUIEvent, this, &PresetTab::guiEvent);
}

void PresetTab::update(ofEventArgs& e)
{
    // nodecam
    if (cam_position)
    {
        nf_pos = ActorManager::instance().getLastSelectedNodeIdentifer();
        nf_pos.findOne(node_pos);
    }
	
    if (cam_look_at)
    {
        nf_look_at = ActorManager::instance().getLastSelectedNodeIdentifer();
        nf_look_at.findOne(node_look_at);
    }
    
    if (use_node_cam)
    {
        ofVec3f pos = node_pos.getGlobalPosition();
        ofVec3f lookAt = node_look_at.getGlobalPosition();
        
        CameraManager::instance().getActiveCamera().setGlobalPosition(pos);
        CameraManager::instance().getActiveCamera().lookAt(lookAt);
    }
}

void PresetTab::draw(ofEventArgs& e)
{
    if (use_node_cam) return;
    
    BeginCamera();
    glEnable(GL_DEPTH_TEST);
    if (nf_pos.found())
    {
        ofVec3f pos = node_pos.getGlobalPosition();
        pos.y += 20;
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawLine(node_pos, pos);
        ofDrawBitmapString("[Camera]", pos);
        ofPopStyle();
    }

    if (nf_look_at.found())
    {
        ofVec3f pos = node_look_at.getGlobalPosition();
        pos.y += 20;
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawLine(node_look_at, pos);
        ofDrawBitmapString("[LookAt]", pos);
        ofPopStyle();
    }
    glDisable(GL_DEPTH_TEST);
    EndCamera();
}


void PresetTab::guiEvent(ofxUIEventArgs &e)
{
	// camera
	int choice = getChoice(e, preset_cam_radio);
	if(choice != -1)
	{
		int indices[] = {0, 1, 5};
		int choice = getChoice(preset_cam_radio);
		CameraManager::instance().rollbackDefaultCameraSetting(indices[choice]);
	}
	
	// preset scenes
	const string name = e.widget->getName();
	
	if      (name == "Line") callPreset(0);
	else if (name == "Hasty Chase") callPreset(1);
	else if (name == "HC + Stamp") callPreset(2);
	else if (name == "HC + Stamp + Natto") callPreset(3);
	else if (name == "HC + Future") callPreset(4);
	else if (name == "Line + Future") callPreset(5);
	else if (name == "Particles + Future") callPreset(6);
	else if (name == "Particles") callPreset(7);
}


void PresetTab::callPreset(size_t preset_id)
{
	disableAllScene();
	
	static const size_t id_line = SceneManager::instance().findtSceneIndex("Line");
	static const size_t id_hasty = SceneManager::instance().findtSceneIndex("Hasty Chase");
	static const size_t id_stamp = SceneManager::instance().findtSceneIndex("Stamp");
	static const size_t id_future = SceneManager::instance().findtSceneIndex("Future");
	static const size_t id_particles = SceneManager::instance().findtSceneIndex("Particles");
	
	switch (preset_id)
	{
		case 0: // Line
			setEnableScene(id_line, true);
			SceneManager::instance().getScene(id_line)->loadPresetXML( ToResourcePath("Settings/presets/preset.lines.xml") );
			break;
			
		case 1: // Hasty Chase
			setEnableScene(id_hasty, true);
			SceneManager::instance().getScene(id_hasty)->loadPreset(0);
			break;
			
		case 2: // HC + Stamp
			setEnableScene(id_hasty, true);
			setEnableScene(id_stamp, true);
			SceneManager::instance().getScene(id_hasty)->loadPreset(0);
			SceneManager::instance().getScene(id_stamp)->loadPreset();
			break;
			
		case 3: // HC + Stamp + Natto
			setEnableScene(id_hasty, true);
			setEnableScene(id_stamp, true);
			SceneManager::instance().getScene(id_hasty)->loadPreset(1);
			SceneManager::instance().getScene(id_stamp)->loadPreset();
			break;
			
		case 4: // HC + Future
			setEnableScene(id_hasty, true);
			setEnableScene(id_future, true);
			SceneManager::instance().getScene(id_hasty)->loadPreset(0);
			SceneManager::instance().getScene(id_future)->loadPreset();
			break;
			
		case 5: // Line + Future
			setEnableScene(id_line, true);
			setEnableScene(id_future, true);
			SceneManager::instance().getScene(id_line)->loadPresetXML( ToResourcePath("Settings/presets/preset.lines.xml") );
			SceneManager::instance().getScene(id_future)->loadPreset();
			break;
			
		case 6: // Particles + Future
			setEnableScene(id_particles, true);
			setEnableScene(id_future, true);
			SceneManager::instance().getScene(id_particles)->loadPreset();
			SceneManager::instance().getScene(id_future)->loadPreset();
			break;
			
		case 7: // Particles
			setEnableScene(id_particles, true);
			SceneManager::instance().getScene(id_particles)->loadPreset();
			break;
	}
}

void PresetTab::setEnableScene(size_t idx, bool enable)
{
	// FIXME:
	// idx:0 is "Actors" scene. it should be not scene but tab.
	if (idx == 0)
		return;
	
	if (idx > SceneManager::instance().getNumScenes())
		return;
	
	SceneManager::instance().getScene(idx)->setEnabled(enable);
	
	// 0:Actors, 1:Preset, 2:Preferences tab don't have enableToggle.
	int ui_offset = 3;
	
	if ((idx + ui_offset) > SceneManager::instance().getNumScenes()+1)
		return;
	
	GetGUI().getSceneTabs().getEnableToggle(idx+ui_offset-1)->setValue(enable);
}

void PresetTab::disableAllScene()
{
	for (int i=0; i<SceneManager::instance().getNumScenes(); i++)
	{
		setEnableScene(i, false);
	}
}
