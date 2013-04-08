#pragma once

#include "ofxUITabbedCanvas.h"
#include "BaseSegment.h"
#include "ramSession.h"

class ControlSegment : public BaseSegment
{
	friend class ramActorsScene;

public:
	
	ControlSegment(string segmentName);
	~ControlSegment();
    
    ramActorUISegmentType getType();
    ofxUICanvasPlus* createPanel(const string targetName);
    
	void onValueChanged(ofxUIEventArgs& e);
	void toggleRecording(const bool bStart);

	bool isRecording();
    
protected:
    
    /// additional variables from BaseSegment...
    
    // ui parts
	ofxUIImageToggle *btnRecordActor;
	
    
    // initialize
	void init();
    
	
    // flags
	bool bRecording;
};
