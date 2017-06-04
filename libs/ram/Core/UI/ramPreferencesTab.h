// 
// ramPreferencesTab.h - RAMDanceToolkit
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

#include "ramGraphics.h"
#include "ramImGuiPanel.h"
#include "ofxUITabbedCanvas.h"

namespace rdtk{
	
	class Preference {
	public:
		bool fullscreen = false;
		bool useShadows = false;
		float floorSize = 600.0;
		float floorGridSize = 50.0;
		int floorStyle = Floor::FLOOR_GRID_LINES;
		ofFloatColor bg = ofFloatColor(0);
		
		inline static Preference& instance()
		{
			if (__instance == NULL) __instance = new Preference;
			return *__instance;
		}
		
	private:
		static Preference *__instance;
		
		Preference() {};
		Preference(const Preference&){}
		Preference& operator=(const Preference&){return *__instance;}
		~Preference() {};
	};
		
	class PreferencesGui : public SceneGui {
	public:
		virtual string getName() const {return "Preference";}
		
		void drawImGui()
		{
			Preference & pref = Preference::instance();
			
			ImGui::LabelText("", "Floor Style");
			ImGui::RadioButton("NONE"	, &pref.floorStyle, Floor::FLOOR_NONE);
			ImGui::SameLine();
			ImGui::RadioButton("GRID"	, &pref.floorStyle, Floor::FLOOR_GRID_LINES);
			ImGui::RadioButton("PLANE"	, &pref.floorStyle, Floor::FLOOR_PLANE);
			ImGui::SameLine();
			ImGui::RadioButton("CHECKER", &pref.floorStyle, Floor::FLOOR_CHECKER_PATTERN);
			
			ImGui::SliderFloat("Floor size", &pref.floorSize, 100, 1000);
			ImGui::SliderFloat("Floor grid size", &pref.floorGridSize, 20, 200);
			
			ImGui::ColorEdit3("background", &pref.bg[0]);
			if (ImGui::Checkbox("Fullscreen", &pref.fullscreen))
				ofSetFullscreen(pref.fullscreen);
			ImGui::SameLine();
			ImGui::Checkbox("useShadow", &pref.useShadows);
		}
	};
	
	class PreferencesTab : public ofxUITab {
	protected:
		ofxUIRadio* floorStyleRadio;
		ofxUILabelToggle* fullscreenToggle;
	public:
		PreferencesTab()
		:ofxUITab("Preferences", false)
		{
			Preference & pref = Preference::instance();
			fullscreenToggle = addLabelToggle("Fullscreen", &pref.fullscreen);
			addLabelToggle("Use shadows", &pref.useShadows);
			addSpacer();
			
			vector<string> floorNames = Floor::getFloorNames();
			floorStyleRadio = addRadio("Floor style", floorNames);
			floorStyleRadio->getToggles()[pref.floorStyle]->setValue(true);
			addSlider("Floor size", 100, 1000, &pref.floorSize);
			addSlider("Floor grid Size", 20, 200, &pref.floorGridSize);
			addSpacer();
			
			addLabel("Background color", OFX_UI_FONT_MEDIUM);
			addSlider("Red", 0, 1, &pref.bg.r);
			addSlider("Green", 0, 1, &pref.bg.g);
			addSlider("Blue", 0, 1, &pref.bg.b);
			
			autoSizeToFitWidgets();
			
			ofAddListener(this->newGUIEvent, this, &PreferencesTab::onValueChanged);
			ofSetFullscreen(pref.fullscreen);
		}
		int getFloorPattern() const {
			return Preference::instance().floorStyle;
		}
		float getFloorSize() const { return Preference::instance().floorSize; }
		float getFloorGridSize() const { return Preference::instance().floorGridSize; }
		
		void update() {
			
			if (fullscreenToggle->getValue() != (ofGetWindowMode() == OF_FULLSCREEN))
			{
				fullscreenToggle->setValue(ofGetWindowMode() == OF_FULLSCREEN);
				fullscreenToggle->stateChange();
			}
			
			EnableShadow(Preference::instance().useShadows);
			ofBackground(Preference::instance().bg);
		}
		
		void onValueChanged(ofxUIEventArgs& e)
		{
			const string widgetName = e.widget->getName();
			
			if (widgetName == "Fullscreen")
			{
				ofSetFullscreen(fullscreenToggle->getValue());
			}
		}
	};
	
}

typedef rdtk::PreferencesTab RAMDEPRECATED(ramPreferencesTab);
