#pragma once

#include "ramSession.h"
#include "ramTSVCoder.h"
#include "ofxXmlSettings.h"

class ControlSegment
{
	friend class ramActorsScene;

public:
	
	ControlSegment();
	~ControlSegment();
	ofxUICanvasPlus* createPanel(const ramNodeArray &NA);
	void onValueChanged(ofxUIEventArgs& e);
	
	void toggleRecording(const bool bStart);
	
	void loadCache();
	void saveCache();
	
private:
	
	void reset();
	const string getXMLFilePath() const;
	
	ofxUIImageToggle *btnHideActor;
	ofxUIImageButton *btnResetActor;
	ofxUIImageToggle *btnRecordActor;
	
	ramSession session;
	ofFloatColor jointColor;
	ofPoint position;
	
	ofxXmlSettings XML;
	string name;
	
	/// to encode/decode to tsv file
	ramTSVCoder coder;
	
	bool bHideActor;
	bool bNeedsResetPos;
	bool bRecording;
};
