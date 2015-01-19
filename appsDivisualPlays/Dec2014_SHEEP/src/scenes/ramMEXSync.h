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
	string getName() const { return "ramMEXSync"; }

protected:
	vector<string> scenes;
	ramMotionExtractor	mex;
	ofxOscSender		sender;

};

#endif /* defined(__RAMDanceToolkit__ramMEXSync__) */
