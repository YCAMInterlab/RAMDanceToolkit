#pragma once

#include "ramSession.h"
#include "ofxXmlSettings.h"

class ControlSegment
{
	friend class ramActorsScene;

public:
	
	ControlSegment();
	ofxUICanvasPlus* setup(ofxUICanvasPlus* panel, const ramNodeArray &NA);
	void onValueChanged(ofxUIEventArgs& e);
	
	void load();
	void save();
	
private:
	
	void reset();
	
	ramSession session;
	ofFloatColor jointColor;
	ofPoint position;
	
	bool bHideActor;
	bool bNeedsResetPos;
	bool bRecording;
};
