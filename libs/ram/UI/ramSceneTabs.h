#pragma once
#include "ofxUI.h"
#include "ofxUITabbedCanvas.h"


#pragma mark - ramScenePanel
class ramScenePanel : public ofxUICanvas
{
	
public:
	
	/// override constructor
	// --------------------
	ramScenePanel(float x, float y, float w, float h) : ofxUICanvas(x, y, w, h) {}
	
	/// getter, setter
	// --------------------
	inline void setSceneName(const string name) { sceneName = name; }
	inline string getSceneName() const { return sceneName; }
	
protected:
	
	/// name used for save/load settings
	// --------------------
	string sceneName;
};


#pragma mark - ramSceneTabs
class ramSceneTabs : public ofxUITabbedCanvas
{
public:
	
	/// override to create friendly file name
	// --------------------
	void loadSettings(const string &fileName)
	{
		string baseName = fileName;
		ofStringReplace(baseName, "XML", "xml");
		
		for (int i=0; i<size(); i++) {
			string file = baseName;
			ofStringReplace(file, ".xml", "-" + ofToString(i) + "_"+ ((ramScenePanel*)at(i))->getSceneName() +".xml");
			at(i)->loadSettings(file);
		}
	}
	
	/// override to load friendly file name
	// --------------------
	void saveSettings(const string &fileName)
	{
		string baseName = fileName;
		ofStringReplace(baseName, "XML", "xml");
		
		for (int i=0; i<size(); i++) {
			string file = baseName;
			ofStringReplace(file, ".xml", "-" + ofToString(i) + "_"+ ((ramScenePanel*)at(i))->getSceneName() +".xml");
			at(i)->saveSettings(file);
		}
	}
};
