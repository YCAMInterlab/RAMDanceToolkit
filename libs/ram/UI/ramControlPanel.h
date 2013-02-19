#pragma once

#include "ofxUITabbedCanvas.h"

#include "ramControlPanelBase.h"

#include "ramSharedData.h"
#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramControllable.h"
#include "ramBaseScene.h"

class ramBaseScene;
class ramControllable;

class ramOfxUIControlPanel : public ramControlPanelBase
{
	
public:
	
	float kDim, kXInit, kLength;
	
	static ramOfxUIControlPanel& instance();
	virtual ~ramOfxUIControlPanel() {}
	
	void setup();
	void update(ofEventArgs &e);
	
	void setupSceneToggles(vector<ramBaseScene*>& scenes);
	void addPanel(ramControllable* control);
	void reloadCameraSetting(const int index);
	
	inline ofColor getBackgroundColor() { return ofColor(mR, mG, mB); }
	inline int getFloorPattern() { return mFloorPattern; }
	inline float getFloorSize() { return mFloorSize; }
	inline float getGridSize() { return mGridSize; }
	
	inline ofxUITabbedCanvas& getTabbedCanvas() { return mTabbedCanvas; }
	inline ofxUIToggleMatrix* getSceneToggles() { return mSceneToggles; }
	
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
		ofxUIButton *button = current_panel->addButton(name, false, 30, 30);
		
		struct Listener
		{
			ofxUIButton *button;
			struct ICallable *callable;
			
			Listener(ofxUIButton *button, ICallable *callable) : button(button), callable(callable) {}
			~Listener() { delete button; delete callable; }
			
			void handle(ofxUIEventArgs &e)
			{
				if (!button->getValue()) return;
				callable->call();
			}
		};
		
		// FIXME: memory leak
		Listener *e = new Listener(button, new Callback<Functor>(functor));
		ofAddListener(current_panel->newGUIEvent, e, &Listener::handle);
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
	
	float mR, mG, mB;
	bool mUseBgSlider;
	bool enableShadow;
	int mFloorPattern;
	float mFloorSize, mGridSize;
	
    ofxUITabbedCanvas mTabbedCanvas;
	ofxUIToggleMatrix *mSceneToggles;
	ofxUICanvas *mPanelGeneral;
	ofxUILabel *mLabelCamPos;
	
	ofxUICanvas *current_panel;
	
	vector<ramBaseScene*> *scenes;
	
	static ramOfxUIControlPanel *_instance;
	
	ramOfxUIControlPanel();
};

struct ramControlPanel : public ramOfxUIControlPanel {};

inline ramControlPanel& ramGetGUI() { return (ramControlPanel&)ramOfxUIControlPanel::instance(); }

