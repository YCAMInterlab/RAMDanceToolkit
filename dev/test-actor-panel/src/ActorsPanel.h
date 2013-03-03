#pragma once

#include "ramBaseScene.h"
#include "ramTSVCoder.h"

#include "ControlSegment.h"

class ActorsPanel : public ramBaseScene
{
public:
	
	ActorsPanel();
	
	void setupControlPanel();
	void setup();
	void update();
	void draw();
	void drawHUD();
	
	void onActorSetup(const ramActor &actor);
	void onRigidSetup(const ramRigidBody &rigid);
	void onActorExit(const ramActor &actor);
	void onRigidExit(const ramRigidBody &rigid);
	void onEnabled();
	const string getName();
	
	
private:
	
	void addControlSegment(const ramNodeArray &NA);
	void removeControlSegment(const ramNodeArray &NA);
	void rebuildControlPanel();

	
	/// ActorsPanel
	ofxUICanvasPlus *mLocalPanel;
	
	
	/// ControlSegment map
	map<string, ControlSegment*> mSegmentsMap;
	typedef map<string, ControlSegment*>::iterator SegmentsIter;
	
	
	/// to encode/decode to tsv file
	ramTSVCoder coder;
	
	
	/// fonts
	ofTrueTypeFont font;
	float fontSize;
	
	
	/// update flags
	bool mNeedUpdatePanel;
	bool bRecording;
	
	
	/// to store actor color
	ofxXmlSettings XML;
	
};



