#pragma once

#include "ramSession.h"
#include "ofxXmlSettings.h"

class ControlSegment
{
	friend class ramActorsScene;

public:
	
	ControlSegment();
	~ControlSegment();
	ofxUICanvasPlus* createPanel(const ramNodeArray &NA);
	void onValueChanged(ofxUIEventArgs& e);
	
	void loadCache();
	void saveCache();
	
private:
	
	ofxUIImageToggle *btnHideActor;
	ofxUIImageButton *btnResetActor;
	ofxUIImageToggle *btnRecordActor;
	
	ofxXmlSettings XML;
	string name;
	
	void reset();
	const string getXMLFilePath() const;
	
	ramSession session;
	ofFloatColor jointColor;
	ofPoint position;
	
	bool bHideActor;
	bool bNeedsResetPos;
	bool bRecording;
};
