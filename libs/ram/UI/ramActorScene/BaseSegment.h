#pragma once

#include "ofxUITabbedCanvas.h"
#include "ramSession.h"
#include "ramTSVCoder.h"
#include "ramControlPanel.h"

enum ramActorUISegmentType
{
    RAM_UI_SEGMENT_TYPE_CONTROL = 0,
    RAM_UI_SEGMENT_TYPE_PLAYBACK = 1
};

class BaseSegment
{
    
    friend class ramActorsScene;
    
public:
    
    /// load / save actor's color, position, visibility....
    void loadCache();
    void saveCache();
    
    
    /// control panel definition
    virtual ofxUICanvasPlus* createPanel(const string targetName) = 0;
    virtual ramActorUISegmentType getType() = 0;

    string getName() const;
    
    inline void setVisibility(bool b) { bHideActor = b; }
    inline bool isVisible() const { return !bHideActor; }
    
protected:
    
    ///
    virtual void onValueChanged(ofxUIEventArgs& e) = 0;
    virtual void init();
    
    
    string getCacheFilePath() const;
    
    
    /// flags
    ofxUIImageToggle *btnHideActor;
	ofxUIImageButton *btnResetActor;
    bool bHideActor;
    bool bNeedsResetPos;
    
    
    /// load / save motion data
    ramSession session;
    ofxXmlSettings XML;
    ramTSVCoder coder;
    
    
    /// 
    string name;
    ofFloatColor jointColor;
    ofPoint position;
    
    
    //
    int segmentType;
};