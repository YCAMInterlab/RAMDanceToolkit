#pragma once
#include "ofxUI.h"
#include "ofxUITabbedCanvas.h"

#pragma mark - ramScenePanel
class ramScenePanel : public ofxUITab
{
public:

	/// getter, setter
	// --------------------
	inline void setSceneName(const string name) { setTabName(name); }
	inline string getSceneName() const { return getTabName(); }
};

#pragma mark - ramSceneTabs
class ramSceneTabs : public ofxUITabbedCanvas
{
public:

};