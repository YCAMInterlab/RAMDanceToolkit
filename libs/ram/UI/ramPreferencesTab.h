#pragma once

#include "ofxUITabbedCanvas.h"

class ramPreferencesTab : public ofxUITab {
protected:
	ofxUIRadio* floorStyleRadio;
	bool fullscreen, useShadows;
	float floorSize, floorGridSize;
	int floorStyle;
	float bgHue, bgSaturation, bgBrightness;
public:
	ramPreferencesTab()
	:ofxUITab("Preferences", false)
	,fullscreen(true)
	,useShadows(true)
	,floorStyle(ramFloor::FLOOR_GRID_LINES)
	,floorSize(600.0)
	,floorGridSize(50.0)
	,bgHue(0)
	,bgSaturation(0)
	,bgBrightness(0)
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
		addSlider("Hue", 0, 1, &bgHue);
		addSlider("Saturation", 0, 1, &bgSaturation);
		addSlider("Brightness", 0, 1, &bgBrightness);
		
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
		ofBackground(ofFloatColor::fromHsb(bgHue, bgSaturation, bgBrightness));
	}
};
