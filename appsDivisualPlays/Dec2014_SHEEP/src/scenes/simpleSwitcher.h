//
//  simpleSwitcher.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/22.
//
//

#ifndef __RAMDanceToolkit__simpleSwitcher__
#define __RAMDanceToolkit__simpleSwitcher__

#include "ramMain.h"

class simpleSwitcher{
public:
	
	void setup();
	void update();
	void draw();
	
	ofxUICanvas* selector_hakoniwa;
	ofxUICanvas* selector_display;
	ofxUITextInput* addressInput;
	void guiEvent(ofxUIEventArgs &ev);
	
	
	//Send to
	vector<string> hakoniwas;
	
	ofxOscSender sender;
	
	bool mEnable;
	bool displays[4];
	bool mAutoShuffle;
	
	void sendMessage();
	
};

#endif /* defined(__RAMDanceToolkit__simpleSwitcher__) */
