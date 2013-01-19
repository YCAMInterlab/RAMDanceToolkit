#pragma once
#include "ofxAutoControlPanel.h"

class ramSceneBase
{
public:
	ramSceneBase() : bEnabled(false) {}
	
	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { bEnabled ^= true; }
	
	string getSceneName() { return sceneName.empty() ? "no name" : sceneName; }
	
protected:
	
	ofxAutoControlPanel *guiPtr;
	
	string sceneName;
	bool bEnabled;
};