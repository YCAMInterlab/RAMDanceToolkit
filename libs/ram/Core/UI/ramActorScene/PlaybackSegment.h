// 
// PlaybackSegment.h - RAMDanceToolkit
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

#include "BaseSegment.h"

class PlaybackSegment : public BaseSegment
{
	
public:
	
	PlaybackSegment(const string& segmentName);
	~PlaybackSegment();
	
    ramActorUISegmentType getType() const;
    ofxUICanvasPlus* createPanel(const string& targetName);
	
	void update();
	
    void pause(bool bPause);
    void deleteSelf();
    
	void loadCache();
	void saveCache();

	inline bool isPlaying() const { return !bPaused; }
    
private:
	
    void onValueChanged(ofxUIEventArgs& e);
    void init();
	
	ofxUIImageToggle *btnPlayActor;
	ofxUIImageButton *btnCueActor;
	ofxUIImageButton *btnDeleteActor;
	
    bool bPaused;
};



