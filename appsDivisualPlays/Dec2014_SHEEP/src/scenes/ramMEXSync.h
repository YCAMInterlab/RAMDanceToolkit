//
//  ramMEXSync.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/19.
//
//

#ifndef __RAMDanceToolkit__ramMEXSync__
#define __RAMDanceToolkit__ramMEXSync__

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "simpleSwitcher.h"

enum actorPresetMode{
	ACTPRE_STANDARD,
	ACTPRE_SOLO,
	ACTPRE_DUO,
	ACTPRE_TRIO,
	ACTPRE_SET_A,
	ACTPRE_SET_B,
	ACTPRE_SET_C,
	ACTPRE_SET_D,
	ACTPRE_SET_E
};

class oscPreview{
public:
	oscPreview(){};
	~oscPreview(){};
	ofxOscMessage msg;
	vector<float> numValue;
	vector<float> valueClamp;
};

class ramMEXSync : public ramBaseScene{
public:
	ramMEXSync();
	~ramMEXSync(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	ofxUICanvasPlus* gui;
	ofxUITextInput* targScene;
	ofxUITextInput* uiName;
	ofxUIRadio* sceneRadio;
	
	ofxUICanvas* presetGui;
	ofxUIRadio* actPresetRadio;
	
	ofxUICanvas* blockLauncher;
	void blockLaunch(ofxUIEventArgs& e);
	
	simpleSwitcher switcher;
	
	float mUIRemote_float;
	float mUIRemote_fOrder;
	bool mUIRemote_toggle;
	
	inline
	string getName() const { return "Presentor"; }

	const string ip_1 = "192.168.20.2";
	const string ip_2 = "192.168.20.3";
	
	bool mSignal_Enable;
	bool mSignal_DispA;
	bool mSignal_DispB;
	bool mRemotePreset;
	
	int getterDelay;
	void getExtractor();
	void setExtractor();

	void onEnabled();
	void onDisabled();
	
	string currentScene[4];
	
	vector<oscPreview*> previews;
	
	//for Remote Functions
	void setActorSort(vector<string> list);
	void setScene(string scene,bool enable,bool A,bool B);
	void setActorPreset(actorPresetMode mode);
	
protected:
	vector<bool>	enabled;
	vector<string> actorPreset;
	
	vector<string> presetName;
	vector<string> scenes;
	vector<string> scenes_pair;
	ramMotionExtractor	mex;
	ofxOscSender		sender;

	ramOscReceiveTag receiver;
};

#endif /* defined(__RAMDanceToolkit__ramMEXSync__) */
