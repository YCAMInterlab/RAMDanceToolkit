//
//  FourPoints_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#ifndef FourPoints_ext_h
#define FourPoints_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class FourPoints_ext : public ramBaseScene{
public:
	
	inline string getName() const {return "FourPoints_ext";}
	
	FourPoints_ext(){};
	~FourPoints_ext(){};
	
	void setupControlPanel();
	void update();
	void draw();
	
private:
	
	ramMotionExtractor mex;
	float twistResolution, twistExtensionWidth, twistExtensionHeight, pointSize;
};

#endif /* FourPoints_ext_h */
