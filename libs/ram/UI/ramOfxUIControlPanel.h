#pragma once

#ifdef RAM_GUI_SYSTEM_OFXUI

#include "ofxUITabbedCanvas.h"

#include "ramGlobal.h"
#include "ramGraphics.h"
#include "ramCameraManager.h"

#include "ramPreferencesTab.h"
#include "ramPresetTab.h"
#include "ramPlaybackTab.h"

class ramBaseScene;
class ramControllable;

class ramOfxUIControlPanel
{

public:

	const float kDim;
	const float kXInit;
	const float kLength;

	static ramOfxUIControlPanel& instance();
	virtual ~ramOfxUIControlPanel();

	void setup();

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

	void addPanel(ramBaseScene* control,  bool enableable = true);

	ramPreferencesTab& getPreferencesTab();
	ramBaseScene* getActorsScene();
	
	ofxUITabbedCanvas& getSceneTabs();

	//

	void update(ofEventArgs &e);
	void guiEvent(ofxUIEventArgs &e);

	//

	ofxUICanvasPlus* getCurrentUIContext();
	
private:

	static ramOfxUIControlPanel *_instance;

	int mFloorPattern;
	float mFloorSize, mGridSize;

	int camera_preset, camera_preset_t;
	
	ramPresetTab presetTab;
	ramPreferencesTab preferencesTab;
	ramPlaybackTab playbackTab;

	// bomisutaro!
	// can't be statically allocated due to inheritance conflicts from header-only implementation
	ramBaseScene* actorsScene;
	
	ofFloatColor backgroundColor;

	ofxUITabbedCanvas mSceneTabs;

	ofxUICanvasPlus *current_panel;

	vector<ramBaseScene*> scenes;

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