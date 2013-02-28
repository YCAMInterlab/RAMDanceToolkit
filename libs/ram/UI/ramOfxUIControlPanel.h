#pragma once

#include "ramControlPanelBase.h"

#ifdef RAM_GUI_SYSTEM_OFXUI

#include "ofxUITabbedCanvas.h"

#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramSceneTabs.h"

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

	template <typename Functor>
	void addButton(const string& name, const Functor &functor)
	{
		ofxUIButton *button = current_panel->addButton(name, false, 30, 30);

		// FIXME: memory leak
		ButtonEventListener *e = new ButtonEventListener(button, new Callback<Functor>(functor));
		ofAddListener(current_panel->newGUIEvent, e, &ButtonEventListener::handle);
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

	void addPanel(ramControllable* control);
	void setupSceneToggles(vector<ramBaseScene*>& scenes);
	void reloadCameraSetting(const int index);

	inline ofColor getBackgroundColor() { return backgroundColor; }
	inline int getFloorPattern() { return mFloorPattern; }
	inline float getFloorSize() { return mFloorSize; }
	inline float getGridSize() { return mGridSize; }

	inline ramSceneTabs& getSceneTabs() { return mSceneTabs; }
	inline ofxUIToggleMatrix* getSceneToggles() { return mSceneToggles; }

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

	ramSceneTabs mSceneTabs;
	ofxUIToggleMatrix *mSceneToggles;

	ofxUICanvas *current_panel;

	vector<ramBaseScene*> *scenes;

	struct ButtonEventListener
	{
		ofxUIButton *button;
		struct ICallable *callable;

		ButtonEventListener(ofxUIButton *button, ICallable *callable) : button(button), callable(callable) {}
		~ButtonEventListener() { delete button; delete callable; }

		void handle(ofxUIEventArgs &e)
		{
			if (!button->getValue()) return;

			callable->call();
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