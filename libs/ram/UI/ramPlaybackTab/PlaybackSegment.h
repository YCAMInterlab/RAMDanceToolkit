#pragma once

#include "ofxUITabbedCanvas.h"
#include "ramSession.h"
#include "ramTSVCoder.h"

class PlaybackSegment
{
	friend class ramPlaybackTab;

public:
	
	PlaybackSegment();
	~PlaybackSegment();
	
	ofxUICanvasPlus* createPanel(const ramSession &session);
	void onValueChanged(ofxUIEventArgs& e);
	
	void loadCache();
	void saveCache();
	
private:
	
	void reset();
	const string getXMLFilePath() const;
	
	ofxUIImageToggle *btnHideActor;
	ofxUIImageButton *btnResetActor;
	ofxUIImageToggle *btnDeleteActor;
	
	ramSession session;
	ofFloatColor jointColor;
	ofPoint position;
	
	ofxXmlSettings XML;
	string name;
	
	/// to encode/decode to tsv file
	ramTSVCoder coder;
	
	bool bHideActor;
	bool bNeedsResetPos;
	bool bDelete;
};
