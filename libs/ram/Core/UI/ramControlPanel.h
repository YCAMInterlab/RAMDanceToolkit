// 
// ramControlPanel.h - RAMDanceToolkit
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

#define RAM_GUI_SYSTEM_OFXUI

#ifdef RAM_GUI_SYSTEM_OFXUI

#include "ofxUITabbedCanvas.h"

#include "ramGlobal.h"
#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramPreferencesTab.h"
#include "ramPresetTab.h"

class ramUnit;
class ramControllable;

class ramControlPanel
{

public:

	const float kDim;
	const float kXInit;
	const float kLength;

	static ramControlPanel& instance();
	virtual ~ramControlPanel();

	void setup(bool usePresetScenes);

	// simple GUI
	void addPanel(ofxUITab& tab);
	void addSection(const string& name);
	void addSeparator();
	void addLabel(const string& content);
	ofEvent<ofEventArgs>& addButton(const string& name);
	void addToggle(const string& name, bool *value);
	void addMultiToggle(const string& name, const vector<string>& content, int *value);
	ofxUIRadio* addRadioGroup(const string& name, const vector<string>& content, int *value);
	void addDropdown(const string& name, const vector<string>& content, int *value);
	void addSlider(const string& name, float min_value, float max_value, float *value);
	void addColorSelector(const string& name, ofFloatColor *value);

	void remove(const string& name);

	// load & save

	void save(const string& path);
	void load(const string& path);
	
	// for internal use

	void addPanel(ramUnit* control,  bool enableable = true);

	ramPreferencesTab& getPreferencesTab();
	
	ofxUITabbedCanvas& getSceneTabs();

	//

	void update(ofEventArgs &e);
	void guiEvent(ofxUIEventArgs &e);

	//

	ofxUICanvasPlus* getCurrentUIContext();
	
	void setUsePresetScenes(bool bUse);
	
private:
	static ramControlPanel *_instance;
	
	ofxUICanvasPlus *current_panel;
	ofxUITabbedCanvas mSceneTabs;
	
	ramPresetTab presetTab;
	ramPreferencesTab preferencesTab;

	vector<ramUnit*> scenes;

	ramControlPanel();
};

inline ramControlPanel& ramGetGUI() { return (ramControlPanel &) ramControlPanel::instance(); }

inline void ramSaveSettings(const string filename)
{
	ramGetGUI().save(filename);
}

inline void ramLoadSettings(const string filename)
{
	ramGetGUI().load(filename);
}

#endif