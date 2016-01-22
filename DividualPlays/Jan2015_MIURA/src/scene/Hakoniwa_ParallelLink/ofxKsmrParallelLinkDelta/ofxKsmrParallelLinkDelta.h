//
//  ofxKsmrParallellinkDelta.h
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/07/18.
//
//

#ifndef __sheep_ramSceneTest__ofxKsmrParallellinkDelta__
#define __sheep_ramSceneTest__ofxKsmrParallellinkDelta__

#include "ofMain.h"

class ofxKsmrParallellinkDelta{
public:

	void setup(float Radius,float armLength,float arm2Length,
			   float height,float plotRadius);
	void draw();
	void setPlot(ofVec3f pos);

	float		arm_length;
	float		arm2_length;
	float		radius;
	float		plot_radius;
	ofNode		actuator[3];
	ofNode		arm[3];
	ofNode		plot;
	ofVec3f		plots[3];
};

#endif /* defined(__sheep_ramSceneTest__ofxKsmrParallellinkDelta__) */
