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
	
	ofxUICanvas sceneList;
	ofxUICanvas gui;
	
	ofxUICanvas dynamicUI;
	string currentDUI;
	
	void setup();
	void listUp();
	void setScene(ofxOscMessage m);
	void setDUI(ofxOscMessage m);
	void event(ofxUIEventArgs &e);
	
	void requestCurrentUI();
	
	ofxUIRadio*			currentScene;
	ofxUIDropDownList*	jointSel;
	ofxUIDropDownList*	actorSel;
	vector<string> scenes_for_test;
	vector<string> ActorList;
	vector<string> jointList;
	
	bool bExclusive;

	ofPtr<dpConductor> conPtr;
};

#endif /* dpConductionGUI_h */
