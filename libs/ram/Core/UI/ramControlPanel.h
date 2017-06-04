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

#include "ramCameraManager.h"
#include "ramPreferencesTab.h"
#include "ramPresetTab.h"

namespace rdtk{
	class Unit;
	class Controllable;
	
	class ControlPanel
	{
		
	public:
		
		const float kDim;
		const float kXInit;
		const float kLength;
		
		static ControlPanel& instance();
		virtual ~ControlPanel();
		
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
		void addIntSlider(const string& name, int min_value, int max_value, int *value);
		void addColorSelector(const string& name, ofFloatColor *value);
		
		void remove(const string& name);
		
		// load & save
		
		void save(const string& path);
		void load(const string& path);
		
		// for internal use
		
		void addPanel(ofPtr<Unit> control,  bool enableable = true);
		
		PreferencesTab& getPreferencesTab();
		
		ofxUITabbedCanvas& getSceneTabs();
		
		//
		
		void update(ofEventArgs &e);
		void guiEvent(ofxUIEventArgs &e);
		
		//
		
		ofxUICanvasPlus* getCurrentUIContext();
		
		void setUsePresetScenes(bool bUse);
		void setVisible(bool b);
	private:
		static ControlPanel *_instance;
		
		ofxUICanvasPlus *current_panel;
		ofxUITabbedCanvas mSceneTabs;
		
		PresetTab presetTab;
		PreferencesTab preferencesTab;
		
		vector<ofPtr<Unit> > scenes;
		
		ControlPanel();
	};
	
	inline ControlPanel& GetGUI() { return (ControlPanel &) ControlPanel::instance(); }
	
	inline void SaveSettings(const string filename)
	{
		GetGUI().save(filename);
	}
	
	inline void LoadSettings(const string filename)
	{
		GetGUI().load(filename);
	}

}

OF_DEPRECATED(rdtk::ControlPanel& ramGetGUI());
OF_DEPRECATED(inline void ramSaveSettings(const string filename));
OF_DEPRECATED(inline void ramLoadSettings(const string filename));

typedef rdtk::Unit RAMDEPRECATED(ramUnit);
typedef rdtk::ControlPanel RAMDEPRECATED(ramControlPanel);

#endif
