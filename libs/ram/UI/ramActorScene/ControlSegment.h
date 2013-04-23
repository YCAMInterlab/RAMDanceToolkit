// 
// ControlSegment.h - RAMDanceToolkit
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
    
	void loadCache();
	void saveCache();

protected:
    
    /// additional variables from BaseSegment...
    
    // ui parts
	ofxUIImageToggle *btnRecordActor;
	
    
    // initialize
	void init();
    
	
    // flags
	bool bRecording;
};
