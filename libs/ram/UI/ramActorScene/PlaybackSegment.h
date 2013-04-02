#pragma once

#include "BaseSegment.h"

class PlaybackSegment : public BaseSegment
{
	
public:
	
	PlaybackSegment(string segmentName);
	~PlaybackSegment();
	
    ramActorUISegmentType getType();
    ofxUICanvasPlus* createPanel(const string targetName);
	
	void update();
	
//	void setVisible(const bool value);
//	void setPlay(const bool value);
//	void cue();
	
	
private:
	
    void onValueChanged(ofxUIEventArgs& e);
    void init();
	
	ofxUIImageToggle *btnPlayActor;
	ofxUIImageButton *btnCueActor;
	ofxUIImageButton *btnDeleteActor;
	
    bool bPlaying;
};



