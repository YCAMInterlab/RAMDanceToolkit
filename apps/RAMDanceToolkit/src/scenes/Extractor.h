//
//  mexTest.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/10/20.
//
//

#ifndef mexTest_h
#define mexTest_h

#include "ramMain.h"

class Extractor : public rdtk::BaseScene
{
public:
	
	rdtk::MotionExtractor mEx;
	
	void setup()
	{
		mEx.setup(this);
	}
	
	void update()
	{
		mEx.update();
	}
	
	void draw()
	{
		rdtk::BeginCamera();
		mEx.draw();
		
		if (mEx.getNumPort() >= 3)
		{
			ofVec3f p1 = mEx.getPositionAt(0);
			ofVec3f p2 = mEx.getPositionAt(1);
			ofVec3f p3 = mEx.getPositionAt(2);
			
			ofDrawLine(p1, p2);
			ofDrawLine(p2, p3);
			ofDrawLine(p3, p1);
		}
		
		rdtk::EndCamera();
	}
	
	void drawImGui()
	{
		mEx.drawImGui();
	}
	
	string getName() const { return "Extractor"; }
};

#endif /* mexTest_h */
