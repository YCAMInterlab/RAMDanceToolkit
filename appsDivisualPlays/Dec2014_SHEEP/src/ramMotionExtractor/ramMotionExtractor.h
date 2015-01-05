//
//  ramMotionExtractor.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#ifndef __RAMDanceToolkit__ramMotionExtractor__
#define __RAMDanceToolkit__ramMotionExtractor__

#include "ramMain.h"

class ramMotionExtractor{
public:

	void setup(ramBaseScene* scene_);

	void update();
	void draw();

	ofVec3f		getVelocity();
	ofVec3f		getEulerVelocity();
	ramNode		getRelativePosition();
	float		getNodeDistance();

	ramBaseScene* mScenePtr;

	ramNodeFinder finder[2];
};

#endif /* defined(__RAMDanceToolkit__ramMotionExtractor__) */
