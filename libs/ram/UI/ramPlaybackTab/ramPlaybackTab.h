#pragma once

#include "ofxUITabbedCanvas.h"
#include "ramTSVCoder.h"
#include "ramGlobal.h"
#include "ramSession.h"


class PlaybackSegment;


#pragma mark -
#pragma mark ramPlaybackTab
class ramPlaybackTab : public ofxUITab, public ramGlobalShortcut
{

public:
	
	///
	ramPlaybackTab();
	~ramPlaybackTab();
	
	
	///
	void setup(ofEventArgs &e);
	void update();
	void drawSessions(ofEventArgs &e);
	
	void addControlSegment(ramSession &src);
	void removeControlSegment(const string segmentName);
	
private:
	
	///
	void onFileDrop(ofDragInfo &e);
	void onValueChanged(ofxUIEventArgs &e);
	
	
	///
	void rebuildControlPanel();
	void createPanelHeader();
	
	
	///
	ofxUILabelButton *btnLoadFile;
	ofxUILabelButton *btnHideAll;
	ofxUILabelButton *btnPauseAll;
	ofxUILabelButton *btnDeleteAll;

	
	///
	map<string, PlaybackSegment*> mSegmentMap;
	typedef map<string, PlaybackSegment*>::iterator SegmentsIter;
	
	
	///
	ramTSVCoder coder;
	
	
	/// button state
	bool bPlayAll;
	
	
	/// update flags
	bool mNeedUpdatePanel;
};





#pragma mark -
#pragma mark PlaybackSegment
class PlaybackSegment
{
	friend class ramPlaybackTab;
	
public:
	
	PlaybackSegment();
	~PlaybackSegment();
	
	ofxUICanvasPlus* createPanel(const ramSession &src);
	void onValueChanged(ofxUIEventArgs& e);
	
	void update();
	
	void loadCache();
	void saveCache();
	
	void setVisible(const bool value);
	void setPlay(const bool value);
	void cue();
	
	
private:
	
	void reset();
	const string getXMLFilePath() const;
	
	ofxUIImageToggle *btnHideActor;
	ofxUIImageToggle *btnPlayActor;
	ofxUIImageButton *btnCueActor;
	ofxUIImageButton *btnDeleteActor;
	ofxUISlider *sliderProgress;
	float progress;
	
	ramSession session;
	ofFloatColor jointColor;
	ofPoint position;
	
	ofxXmlSettings XML;
	string name;
	
	bool bHideActor;
	bool bPlaying;
	bool bDelete;
	bool bNeedsResetPos;
	
	ramPlaybackTab *parent;
};


