//
//  ofxHexGrid.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/15.
//
//

#ifndef __RAMDanceToolkit__ofxHexGlid__
#define __RAMDanceToolkit__ofxHexGlid__

#include "ofMain.h"


class ofxHexGrid{
public:

	ofxHexGrid(){
		mScale = 10.0;
	}

	void setGridScale(float scale);
	ofVec2f getPosition(int x,int y);
	const ofVec2f vX = ofVec2f(0.86602540378,0.5);
	const ofVec2f vY = ofVec2f(0.0,1.0);

protected:

	float mScale;

};

#endif /* defined(__RAMDanceToolkit__ofxHexGlid__) */
