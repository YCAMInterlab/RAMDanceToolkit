#pragma once

#include "BaseSegment.h"
#include "ramActorsScene.h"

class ramActorsScene;
class PlaybackSegment : public BaseSegment
{
	
public:
	
	PlaybackSegment(string segmentName);
	~PlaybackSegment();
	
    ramActorUISegmentType getType();
    ofxUICanvasPlus* createPanel(const string targetName);
	
	void update();
    
//	void setVisible(const bool value);
//	void cue();
	
    void pause(bool bPause);
    void deleteSelf();
    
    inline bool isPlaying() const { return !bPaused; }
	
    ramActorsScene *parent;
    
private:
	
    void onValueChanged(ofxUIEventArgs& e);
    void init();
	
	ofxUIImageToggle *btnPlayActor;
	ofxUIImageButton *btnCueActor;
	ofxUIImageButton *btnDeleteActor;
	
    bool bPaused;
};



