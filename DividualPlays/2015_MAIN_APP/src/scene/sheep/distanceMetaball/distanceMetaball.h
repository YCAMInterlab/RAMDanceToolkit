//
//  distanceMetaball.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#ifndef __RAMDanceToolkit__distanceMetaball__
#define __RAMDanceToolkit__distanceMetaball__

#include "ramMain.h"
#include "ofxMetaballs.h"
#include "ramMotionExtractor.h"
#include "dpConstants.h"
#include <GLUT/GLUT.h>

class distanceMetaball : public ramBaseScene{
public:

	distanceMetaball();
	virtual ~distanceMetaball(){};

	void setupControlPanel();

	void update();
	void draw();

	void onPanelChanged(ofxUIEventArgs& e);

	void drawActor(const ramActor &actor);

	inline
	string getName() const { return "distanceMetaball"; }

	virtual void onEnabled();
	virtual void onDisabled();

private:

	float radius_low,radius_high;
	float resolution;

	bool mTest;
	bool mDrawMEX;
	bool mDrawMetaball;
	float mScale;
	float mSmooth;

	bool mFlipFace;
	bool mPoints;
	
	vector<ofVec3f> pts;

	ofxOscSender* mOscSender;

	MarchingTetrahedrons metaball;
	ramMotionExtractor mex;
};

#endif /* defined(__RAMDanceToolkit__distanceMetaball__) */
