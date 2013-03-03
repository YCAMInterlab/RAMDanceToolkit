#pragma once

#include "ofxUITabbedCanvas.h"

class ramPresetTab : public ofxUITab {
protected:
	ofxUIRadio* cameraPresetRadio;
	int cameraPreset;
	ofxUIToggleMatrix* matrix;
public:
	ramPresetTab();
	void setupCamera(ofEventArgs& e);
	void guiEvent(ofxUIEventArgs &e);
};
