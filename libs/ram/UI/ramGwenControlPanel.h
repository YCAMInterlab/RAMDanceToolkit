#pragma once

#include "ramControlPanel.h"
#include "ramControlPanelBase.h"

#ifdef RAM_GUI_SYSTEM_GWEN

#include "ofxGwen.h"

#include "ofxUITabbedCanvas.h"

#include "ramSharedData.h"
#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramControllable.h"
#include "ramBaseScene.h"

class ramBaseScene;
class ramControllable;

class ramGwenControlPanel : public ramControlPanelBase
{
	
public:
	
	enum
	{
		DEFAULT_WIDGET_WIDTH = 350,
		ELEMENT_MARGIN = 5
	};
	
	static ramGwenControlPanel& instance();
	virtual ~ramGwenControlPanel() {}
	
	void setup();
	void update(ofEventArgs &e);
	
	void setupSceneToggles(vector<ramBaseScene*>& scenes);
	void addPanel(ramControllable* control);
	void reloadCameraSetting(const int index);
	
	inline ofColor getBackgroundColor() { return mBGColor; }
	inline int getFloorPattern() { return mFloorPattern; }
	inline float getFloorSize() { return mFloorSize; }
	inline float getGridSize() { return mGridSize; }
	
//	inline ofxUITabbedCanvas& getTabbedCanvas() { return mTabbedCanvas; }
//	inline ofxUIToggleMatrix* getSceneToggles() { return mSceneToggles; }
	
	void guiEvent(ofxUIEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);
	
	
	// simple GUI
	
	void addPanel(const string& name);
	void addSection(const string& name);
	
	void addSeparator();
	
	void addLabel(const string& content);

	template <typename Functor>
	void addButton(const string& name, const Functor &functor)
	{
		Gwen::Controls::Button *obj = new Gwen::Controls::Button(getCurrentSection());
		obj->SetText(name);
		
		obj->Dock(Gwen::Pos::Top);
		obj->SetSize(100, 30);
		obj->SetMargin(Gwen::Margin(5, 5, 5, 5));
		
		registerCallback(obj, obj->onPress, functor);
	}

	template <typename Functor>
	void addToggle(const string& name, const Functor &functor)
	{
		pushSection(name, 35);
		
		Gwen::Controls::CheckBoxWithLabel* obj = new Gwen::Controls::CheckBoxWithLabel(getCurrentSection());
		
		obj->Label()->SetText(name);
		obj->Dock(Gwen::Pos::Top);
		obj->SetMargin(Gwen::Margin(5, 5, 5, 5));
		
//		registerCallback(obj->Checkbox(), obj->Checkbox()->onCheckChanged, functor, "bool");
		
		assert(false);
		
		popSection();
	}
	
	
	void addToggle(const string& name, bool *value);
	void addMultiToggle(const string& name, const vector<string>& content, int *value);
	void addRadioGroup(const string& name, const vector<string>& content, int *value);
	void addDropdown(const string& name, const vector<string>& content, int *value);
	
	void addSlider(const string& name, float min_value, float max_value, float *value);
	
	void addColorSelector(const string& name, ofFloatColor *value);

	void remove(const string& name);
	
	//
	
private:
	
	static ramGwenControlPanel *_instance;
	
	
	//
	
	class Separator;
	
	ofxGwen gwen;
	
	Gwen::Controls::DockBase *dock;
	Gwen::Controls::Base* getCurrentSection();

	void pushSection(const string& name, int height);
	void popSection();
	
	stack<Gwen::Controls::Base*> section_stack;

	//
	
	bool mPause;
	
	ofFloatColor mBGColor;
    int mFloorPattern, mCameraPreset;
    float mFloorSize, mGridSize;
	
	bool enableShadow;
	int camera_preset, camera_preset_t;

	string getFullName();
	
	//
	
	vector<ramBaseScene*> *scenes;
	
	struct CallbackHandler : public Gwen::Event::Handler
	{
		void handle(Gwen::Controls::Base* obj)
		{
			if (obj->UserData.Exists("cb"))
			{
				ICallable *callback = obj->UserData.Get<ICallable*>("cb");
				callback->call();
			}
		}
	};
	
	template <typename Functor>
	void registerCallback(Gwen::Controls::Base* obj, Gwen::Event::Caller &caller, const Functor &functor)
	{
		static struct CallbackHandler evt;
		ICallable *callback = new Callback<Functor>(functor);
		obj->UserData.Set<ICallable*>("cb", callback);
		caller.Add(&evt, &CallbackHandler::handle);
	}

	
	ramGwenControlPanel();
};

struct ramControlPanel : public ramGwenControlPanel {};

inline ramControlPanel& ramGetGUI() { return (ramControlPanel&)ramGwenControlPanel::instance(); }

inline void ramSaveSettings(const string filename)
{
	ramGetGUI().save(filename);
}

inline void ramLoadSettings(const string filename)
{
	ramGetGUI().load(filename);
}

#endif