//
//  Chaser_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#ifndef Chaser_ext_h
#define Chaser_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class Chaser_ext : public ramBaseScene{
public:
	
	inline string getName() const {"Chaser_ext";}
	
	void setupControlPanel();
	void update();
	void draw();

	void addNewBuffer(string name);
	
protected:
	
	map<string, ramNodeArrayBuffer> buffers;
	ramMotionExtractor mex;
};

#endif /* Chaser_ext_h */
