//
//  dpConductionGUI.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#ifndef dpConductionGUI_h
#define dpConductionGUI_h

#include "ofMain.h"
#include "ofxUI.h"
#include "dpConductor.h"

class dpConductionGUI{
public:
	
	ofxUICanvas gui;
	
	void setup();
	void event(ofxUIEventArgs &e);
	
	ofxUIDropDownList* currentScene;
	vector<string> scenes_for_test;
	
	bool bExclusive;

	ofPtr<dpConductor> conPtr;
};

#endif /* dpConductionGUI_h */
