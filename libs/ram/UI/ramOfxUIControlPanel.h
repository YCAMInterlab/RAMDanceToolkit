#pragma once

#ifdef RAM_GUI_SYSTEM_OFXUI

#include "ofxUITabbedCanvas.h"

#include "ramGraphics.h"
#include "ramCameraManager.h"

class ramBaseScene;
class ramControllable;

class ramPresetTab : public ofxUITab
{
protected:
	ofxUIRadio* cameraPresetRadio;
	int cameraPreset;
	ofxUIToggleMatrix* matrix;
public:
	ramPresetTab()
	:ofxUITab("Presets", false)
	,cameraPreset(1)
	{
		// should probably be a list of named presets instead of a grid
		matrix = addToggleMatrix("Presets", 4, 4);
		matrix->setAllowMultiple(false);
		addSpacer();
		
		vector<string> cameraPresetNames;
		cameraPresetNames.push_back("Low");
		cameraPresetNames.push_back("High");
		cameraPresetNames.push_back("Overhead");
		cameraPresetRadio = addRadio("Camera position", cameraPresetNames);
		cameraPresetRadio->getToggles()[cameraPreset]->setValue(true);
		
		autoSizeToFitWidgets();
	}
};

class ramPreferencesTab : public ofxUITab
{
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
		addSlider("Floor size", 100, 1000, &floorGridSize);
		addSlider("Floor grid Size", 20, 200, &floorGridSize);
		addSpacer();
		
		addLabel("Background color", OFX_UI_FONT_MEDIUM);
		addSlider("Hue", 0, 1, &bgHue);
		addSlider("Saturation", 0, 1, &bgSaturation);
		addSlider("Brightness", 0, 1, &bgBrightness);
		
		autoSizeToFitWidgets();
	}
};

class ramPlaybackTab : public ofxUITab
{
public:
	ramPlaybackTab()
	:ofxUITab("Playback", false)
	{
	}
};

class ramActorsTab : public ofxUITab
{
protected:
	bool paused;
public:
	ramActorsTab()
	:ofxUITab("Actors", false)
	{
		addLabelToggle("Paused", &paused);
	}
};

class ramOfxUIControlPanel
{

public:

	const float kDim;
	const float kXInit;
	const float kLength;

	static ramOfxUIControlPanel& instance();
	virtual ~ramOfxUIControlPanel() {}

	void setup();

	// simple GUI
	void addPanel(ofxUITab& tab);
	void addPanel(const string& name);
	void addSection(const string& name);
	void addSeparator();
	void addLabel(const string& content);
	
	ofEvent<ofEventArgs>& addButton(const string& name)
	{
		ofxUIButton *button = current_panel->addButton(name, false, 30, 30);
		
		// FIXME: memory leak
		ButtonEventListener *e = new ButtonEventListener(button);
		ofAddListener(current_panel->newGUIEvent, e, &ButtonEventListener::handle);
		return e->evt;
	}

	void addToggle(const string& name, bool *value);
	void addMultiToggle(const string& name, const vector<string>& content, int *value);
	ofxUIRadio* addRadioGroup(const string& name, const vector<string>& content, int *value);
	void addDropdown(const string& name, const vector<string>& content, int *value);
	void addSlider(const string& name, float min_value, float max_value, float *value);
	void addColorSelector(const string& name, ofFloatColor *value);

	void remove(const string& name);

	// load & save

	void save(const string& path) { getSceneTabs().saveSettings(path); }
	void load(const string& path) { getSceneTabs().loadSettings(path); }

	// for internal use

	void addPanel(ramBaseScene* control);
	void reloadCameraSetting(const int index);

	inline ofColor getBackgroundColor() { return backgroundColor; }
	inline int getFloorPattern() { return mFloorPattern; }
	inline float getFloorSize() { return mFloorSize; }
	inline float getGridSize() { return mGridSize; }

	inline ofxUITabbedCanvas& getSceneTabs() { return mSceneTabs; }

	//

	void update(ofEventArgs &e);
	void guiEvent(ofxUIEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);

	//

	ofxUICanvas* getCurrentUIContext() { return current_panel; }

private:

	static ramOfxUIControlPanel *_instance;

	int mFloorPattern;
	float mFloorSize, mGridSize;

	bool fullScreen;
	bool pause;
	bool enableShadow;
	int camera_preset, camera_preset_t;
	
	ramPresetTab presetTab;
	ramPreferencesTab preferencesTab;
	ramPlaybackTab playbackTab;
	ramActorsTab actorsTab;

	ofFloatColor backgroundColor;

	ofxUITabbedCanvas mSceneTabs;

	ofxUICanvas *current_panel;

	vector<ramBaseScene*> scenes;

	struct ButtonEventListener
	{
		ofxUIButton *button;
		ofEvent<ofEventArgs> evt;

		ButtonEventListener(ofxUIButton *button) : button(button){}
		~ButtonEventListener() { delete button; }

		void handle(ofxUIEventArgs &e)
		{
			if (e.widget != button
				&& !button->getValue()) return;

			static ofEventArgs args;
			ofNotifyEvent(evt, args);
		}
	};

	ramOfxUIControlPanel();
};

struct ramControlPanel : public ramOfxUIControlPanel {};

inline ramControlPanel& ramGetGUI() { return (ramControlPanel &) ramOfxUIControlPanel::instance(); }

inline void ramSaveSettings(const string filename)
{
	ramGetGUI().save(filename);
}

inline void ramLoadSettings(const string filename)
{
	ramGetGUI().load(filename);
}

#endif