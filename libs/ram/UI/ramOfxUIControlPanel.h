#pragma once

#include "ramControlPanelBase.h"

#ifdef RAM_GUI_SYSTEM_OFXUI

#include "ofxUITabbedCanvas.h"

#include "ramGraphics.h"
#include "ramCameraManager.h"

class ramBaseScene;
class ramControllable;

class ramOfxUIControlPanel : public ramControlPanelBase
{

public:

	const float kDim;
	const float kXInit;
	const float kLength;

	static ramOfxUIControlPanel& instance();
	virtual ~ramOfxUIControlPanel() {}

	void setup();

	// simple GUI

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
	void addRadioGroup(const string& name, const vector<string>& content, int *value);
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