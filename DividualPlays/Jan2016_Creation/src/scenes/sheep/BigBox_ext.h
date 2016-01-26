//
//  BigBox_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#ifndef BigBox_ext_h
#define BigBox_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class BigBox_ext : public ramBaseScene{
public:
	
	string getName() const {return "BigBox_ext";}
	
	BigBox_ext(){};
	~BigBox_ext(){};
	
	void setupControlPanel();
	void update();
	void draw();
	
private:
	float boxScale;
	float lineWidth;
	ofFloatColor color;
	ramMotionExtractor mex;
	
#define NUMNODE_BBE 512
	ofNode portNodes[NUMNODE_BBE];
	float portScale[NUMNODE_BBE];
};

#endif /* BigBox_ext_h */
