//
//  ofxHexGrid.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/15.
//
//

#include "ofxHexGrid.h"

void ofxHexGrid::setGridScale(float scale){
	mScale = scale;
}

ofVec2f ofxHexGrid::getPosition(int x,int y){
	y -= (x + 1) / 2;
	return x * vX * mScale + y * vY * mScale;
}