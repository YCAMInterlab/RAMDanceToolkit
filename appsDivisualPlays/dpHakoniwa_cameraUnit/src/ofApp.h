#pragma once

#include "ofMain.h"

#include "dpCameraUnit_Controller.h"
#include "dpSwitchingManager.h"

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

	dpCameraUnit_Controller dpCameraUnit;
	dpSwitchingManager dpSwitcher;

	ofxOscReceiver receiver;

	ofSerial ser;

	bool bDebug;
};
