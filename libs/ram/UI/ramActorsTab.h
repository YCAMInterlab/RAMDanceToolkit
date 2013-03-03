#pragma once

#include "ofxUITabbedCanvas.h"

class ramActorsTab : public ofxUITab {
protected:
	bool paused;
public:
	ramActorsTab()
	:ofxUITab("Actors", false) {
		addLabelToggle("Paused", &paused);
	}
	void update() {
		ramActorManager::instance().setFreezed(paused);
	}
};
