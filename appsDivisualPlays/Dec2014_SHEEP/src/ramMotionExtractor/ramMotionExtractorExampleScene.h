//
//  ramMotionExtractorExampleScene.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/05.
//
//

#ifndef __RAMDanceToolkit__ramMotionExtractorExampleScene__
#define __RAMDanceToolkit__ramMotionExtractorExampleScene__

#include "ramMain.h"
#include "ramMotionExtractor.h"

class ramMotionExtractorExampleScene : public ramBaseScene{
public:
	ramMotionExtractorExampleScene();
	virtual ~ramMotionExtractorExampleScene(){};

	void setupControlPanel();

	void update();
	void draw();

	inline string getName() const {
		return "ramMotionExtractorExampleScene";
	}

private:

	bool mDrawLines;
	bool mDrawTriangle;
	bool mDrawPreview;
	bool mDrawDump;

	void example_drawLines();
	void example_drawTriangles();
	void example_drawDump();

	ramMotionExtractor motionExtractor;

};

#endif /* defined(__RAMDanceToolkit__ramMotionExtractorExampleScene__) */
