//
//  BrokenBody_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#ifndef BrokenBody_ext_h
#define BrokenBody_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"
#include "slidePoint.h"

class BrokenBox_ext{
public:
	slidePoint mPos;
	float v;
	float a;
	bool bFixed = false;
	BrokenBox_ext()
	{
		mPos.speed = 0.333;
		v = 1.0;
		a = 1.1;
	}
	
	void update(ofPoint pos)
	{
		if(bFixed == false){
			mPos.set(pos);
			v = 1.0;
		}
		else {
			v *= a;
			mPos.yAdd(-v);
			
			if(mPos.y < 0.0)mPos.imSetY(0.0);
		}
		
		mPos.update();
	}
};

class BrokenBody_ext : public ramBaseScene{
public:
	
	string getName() const { return "BrokenBody_ext"; }
	
	void setupControlPanel();
	void update();
	void draw();
	
private:
	ofFloatColor bodyColor;
	vector<BrokenBox_ext> mBoxes;
	float thresh = 0.5;
	ramMotionExtractor mex;
};

#endif /* BrokenBody_ext_h */
