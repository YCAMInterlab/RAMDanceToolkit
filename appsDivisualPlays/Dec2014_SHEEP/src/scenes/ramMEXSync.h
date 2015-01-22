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
	
	ofxUICanvas presetGui;
	
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

	vector<oscPreview*> previews;
protected:
	vector<string> scenes;
	vector<string> scenes_pair;
	ramMotionExtractor	mex;
	ofxOscSender		sender;

	ramOscReceiveTag receiver;
};

#endif /* defined(__RAMDanceToolkit__ramMEXSync__) */
