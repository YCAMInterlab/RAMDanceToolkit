#pragma once

#include "ramBaseScene.h"
#include "ControlSegment.h"


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
	
	void onKeyPressed(ofKeyEventArgs &e);
	void onValueChanged(ofxUIEventArgs &e);
	
	void drawNodes(const ramNodeArray &NA); // experimental
	
	void setShowAll(bool showAll);
	bool getShowAll() const;
	
private:
	
	/// internal use
	void addControlSegment(const ramNodeArray &NA);
	void removeControlSegment(const ramNodeArray &NA);
	void rebuildControlPanel();
	void createPanelHeader();

	
	/// instances which are controlled programatically
	ofxUILabelToggle *btnShowAll;
	ofxUILabelToggle *btnPause;
	ofxUILabelToggle *btnRecAll;

	
	/// ActorsPanel
	ofxUICanvasPlus *mLocalPanel;
	
	
	/// ControlSegment map
	map<string, ControlSegment*> mSegmentsMap;
	typedef map<string, ControlSegment*>::iterator SegmentsIter;
	
	
	/// fonts
	ofTrueTypeFont font;
	float fontSize;
	
	
	/// update flags
	bool mNeedUpdatePanel;
	bool bRecording;
	
	
	/// draw method flags
	ofLight light;
	bool bShowAllActor;
	bool bRecAllActor;
	bool bUseSimpleActor;
	bool bUseShading;
};

