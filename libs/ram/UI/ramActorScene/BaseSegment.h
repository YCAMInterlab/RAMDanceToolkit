// 
// BaseSegment.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    virtual void loadCache() = 0;
    virtual void saveCache() = 0;
    
    
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