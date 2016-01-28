//
//  ThreePoints_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/28/16.
//
//

#ifndef ThreePoints_ext_h
#define ThreePoints_ext_h

#include "ramMain.h"

#include "SphereMesh.h"
#include "ramGeometry.h"
#include "ramMotionExtractor.h"

class ThreePoints_ext: public ramBaseScene
{
public:
	static const ofColor cyanPrint;
	static const ofColor magentaPrint;
	static const ofColor yellowPrint;

	ramMotionExtractor mex;

	//ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
    bool mNodeVisibility[ramActor::NUM_JOINTS];

	bool showRects, showCircle, showSpheres, invertSpheres, showCircleBisector, showCenterCircles;
	float pointSize, crossLength, rectRadius, maxInvertRadius, circleResolution;

	void setupControlPanel();

	void setup();

	void update();

	void draw();

	void drawActor(const ramActor &actor);

	void drawRigid(const ramRigidBody &rigid)
	{
	}

	string getName() const
	{
		return "ThreePoints_ext";
	}
};


#endif /* ThreePoints_ext_h */
