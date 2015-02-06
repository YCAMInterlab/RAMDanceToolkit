// 
// CameraMover.h - RAMDanceToolkit
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
#include "ramBaseScene.h"
#include "ofxXmlSettings.h"

class CameraMover : public ramBaseScene
{
public:

    string getName() const { return "Camera Mover"; }

    void setupControlPanel();
    void setup(string path);
    void setup();
    void draw();
    
    void reloadSettings();
    void refreshPanel(ofxUICanvas *panel);
    void onTriggered(ofxUIEventArgs &e);
	
private:

    ofxXmlSettings mSettingXml;
	vector<ramCameraSettings> mSettings;
	ramCameraSettings mCurSetting;
	bool bCameraOn;
	
	ofxUICanvas *mPanel;
	string mXmlPath;
	
};

