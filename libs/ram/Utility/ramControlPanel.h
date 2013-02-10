#pragma once

#include "ofxUITabbedCanvas.h"

#include "ramSharedData.h"
#include "ramGraphics.h"
#include "ramCameraManager.h"
#include "ramCameraSettings.h"

class ramSceneBase;

class ramControlPanel
{
	
private:
	
	const float kDim = 16;
	const float kXInit = OFX_UI_GLOBAL_WIDGET_SPACING;
	const float kLength = 320-kXInit;
	const string kCamSettingFile = "Settings/cam.default_positions.xml";
	
	float mR, mG, mB;
	bool mUseBgSlider;
	int mFloorPattern;
	float mFloorSize, mGridSize;
	
    ofxUITabbedCanvas mTabbedCanvas;
	ofxUIToggleMatrix *mSceneToggles;
	ofxUICanvas *mPanelGeneral;
	ofxUILabel *mLabelCamPos;
	
	static ramControlPanel *_instance;
	
	ramControlPanel();

public:
	
	inline ofColor getBackgroundColor() { return ofColor(mR, mG, mB); }
	inline int getFloorPattern() { return mFloorPattern; }
	inline float getFloorSize() { return mFloorSize; }
	inline float getGridSize() { return mGridSize; }
	
	inline ofxUITabbedCanvas& getTabbedCanvas() { return mTabbedCanvas; }
	inline ofxUIToggleMatrix* getSceneToggles() { return mSceneToggles; }
	
	static ramControlPanel& instance();
	
	virtual ~ramControlPanel() {}
	
	void setup();
	void update(ofEventArgs &e);
	
	void setupSceneToggles(vector<ramSceneBase*>& scenes);
	void reloadCameraSetting(const int index);
	
	void guiEvent(ofxUIEventArgs &e);
};


inline ramControlPanel& ramGetGUI() { return ramControlPanel::instance(); }