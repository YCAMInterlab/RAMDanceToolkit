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

	inline
	string getName() const { return "Presentor"; }

	const string ip_1 = "192.168.20.26";
	const string ip_2 = "192.168.20.3";
	
	bool mSignal_Enable;
	bool mSignal_DispA;
	bool mSignal_DispB;
	
protected:
	vector<string> scenes;
	ramMotionExtractor	mex;
	ofxOscSender		sender;

	ramOscReceiveTag receiver;
};

#endif /* defined(__RAMDanceToolkit__ramMEXSync__) */
