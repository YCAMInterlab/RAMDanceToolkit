// 
// ramActorsScene.h - RAMDanceToolkit
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
#include "ControlSegment.h"
#include "PlaybackSegment.h"
#include "ramActorManager.h"

/*
 almost all things of this class depends on ofxUI
*/
class ramActorsScene : public ramBaseScene
{
public:
	
	ramActorsScene();
	~ramActorsScene();
	
	string getName() const;
	void setupControlPanel();
	void setup();
	void update();
	void draw();
	void drawHUD();
	
	void onActorSetup(const ramActor &actor);
	void onRigidSetup(const ramRigidBody &rigid);
	void onActorExit(const ramActor &actor);
	void onRigidExit(const ramRigidBody &rigid);
	
	void gotMessage(ofMessage &msg);
	void onKeyPressed(ofKeyEventArgs &e);
	void onValueChanged(ofxUIEventArgs &e);
	void onFileDrop(ofDragInfo &e);
    void loadFile(const string filePath);
	
	void showAll(bool showAll);
	void resetPosAll(bool showAll);
    void pauseAll(bool bPause);
    void recAll(bool bStartRec);
	bool getShowAll();
    
	void setNeedsUpdatePanel(const bool needsUpdate);
    bool needsUpdatePanel();
    
	void drawNodes(const ramNodeArray &NA); // experimental
    
    void addSegment(BaseSegment *newSegment);
	void removeControlSegment(const string name);
	
private:
	
	static const int MAX_ACTORS = 4;
	
    /// playback
	ramTSVCoder coder;
    
	
	/// internal use
	void rebuildControlPanel();
	void createPanelHeader();
    
    
	/// instances which are controlled programatically
	ofxUILabelToggle *btnShowAll;
	ofxUILabelToggle *btnPause;
	ofxUILabelToggle *btnRecAll;

	
	/// ActorsPanel
	ofxUICanvasPlus *mLocalPanel;
	
	
	/// ControlSegment map
	map<string, BaseSegment*> mSegmentsMap;
	typedef map<string, BaseSegment*>::iterator SegmentsIter;
	
	
	/// fonts
	ofTrueTypeFont font;
	float fontSize;
	
	
	/// update flags
	bool bNeedUpdatePanel;
	bool bRecording;
	
	
	/// draw method flags
	ofLight light;
	bool bShowAllActor;
	bool bRecAllActor;
	bool bUseSimpleActor;
	bool bUseShading;
};
