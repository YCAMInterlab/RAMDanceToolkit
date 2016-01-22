#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	ofxUICanvas selector_hakoniwa;
	ofxUICanvas selector_display;
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
