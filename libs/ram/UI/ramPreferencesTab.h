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

#include "ofxUITabbedCanvas.h"

class ramPreferencesTab : public ofxUITab {
protected:
	ofxUIRadio* floorStyleRadio; 
	bool fullscreen, useShadows;
	float floorSize, floorGridSize;
	int floorStyle;
	ofFloatColor bg;
public:
	ramPreferencesTab()
	:ofxUITab("Preferences", false)
	,fullscreen(false)
	,useShadows(true)
	,floorStyle(ramFloor::FLOOR_GRID_LINES)
	,floorSize(600.0)
	,floorGridSize(50.0)
	,bg(0)
	{
		addLabelToggle("Fullscreen", &fullscreen);
		addLabelToggle("Use shadows", &useShadows);
		addSpacer();
		
		vector<string> floorNames = ramFloor::getFloorNames();
		floorStyleRadio = addRadio("Floor style", floorNames);
		floorStyleRadio->getToggles()[floorStyle]->setValue(true);
		addSlider("Floor size", 100, 1000, &floorSize);
		addSlider("Floor grid Size", 20, 200, &floorGridSize);
		addSpacer();
		
		addLabel("Background color", OFX_UI_FONT_MEDIUM);
		addSlider("Red", 0, 1, &bg.r);
		addSlider("Green", 0, 1, &bg.g);
		addSlider("Blue", 0, 1, &bg.b);
		
		autoSizeToFitWidgets();
	}
	int getFloorPattern() {
		return getChoice(floorStyleRadio);
	}
	float getFloorSize() { return floorSize; }
	float getFloorGridSize() { return floorGridSize; }
	
	void update() {
		if (fullscreen != (ofGetWindowMode() == OF_FULLSCREEN)) {
			ofSetFullscreen(fullscreen);
		}
		ramEnableShadow(useShadows);
		ofBackground(bg);
	}
};
