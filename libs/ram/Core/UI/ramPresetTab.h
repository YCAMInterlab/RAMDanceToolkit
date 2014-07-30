// 
// ramPresetTab.h - RAMDanceToolkit
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
#include "ramNodeFinder.h"

class ramPresetTab : public ofxUITab
{
    
protected:
    
    // preset scenes
	ofxUIToggleMatrix* matrix;
    
    
    // preset camera positions
    ofxUIRadio* preset_cam_radio;
	int preset_cam_index;
    
    
    // nodecam
    bool use_node_cam;
    bool cam_position;
    bool cam_look_at;
    
    ramNodeFinder nf_pos;
    ramNodeFinder nf_look_at;
    
    ramNode node_pos;
    ramNode node_look_at;
    
	// preset scenes
	void callPreset(size_t preset_id);
	void setEnableScene(size_t idx, bool enable);
	void disableAllScene();
	
public:
    
	ramPresetTab();
	void setup(bool usePresetScenes);
	void update(ofEventArgs& e);
	void draw(ofEventArgs& e);
	void guiEvent(ofxUIEventArgs &e);
    
};
