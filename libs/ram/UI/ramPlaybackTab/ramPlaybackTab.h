#pragma once

#include "ofxUITabbedCanvas.h"
#include "PlaybackSegment.h"
#include "ramTSVCoder.h"
#include "ramGlobal.h"

class ramPlaybackTab : public ofxUITab
{

public:
	
	
	///
	ramPlaybackTab();
	~ramPlaybackTab();
	
	
	///
	void update();
	void drawSessions(ofEventArgs &e);

	
	///
	void onFileDrop(ofDragInfo &e);
	
	
private:
	
	
	///
	void rebuildControlPanel();
	void addControlSegment(const ramSession &session);
	void removeControlSegment(const ramSession &session);
	void createPanelHeader();
	
	
	///
	map<string, PlaybackSegment*> mSegmentMap;
	typedef map<string, PlaybackSegment*>::iterator SegmentsIter;
	
	map<string, ramSession>mSessionsMap;
	typedef map<string, ramSession>::iterator SessionsIter;
	
	
	///
	ramTSVCoder coder;
	
	
	/// button state
	bool bPlayAll;
	
	
	/// update flags
	bool mNeedUpdatePanel;
};
