//
//  ParallelLinkManager.h
//  motorTester
//
//  Created by Ovis aries on 2014/11/19.
//
//

#ifndef __motorTester__ParallelLinkManager__
#define __motorTester__ParallelLinkManager__

#include "ofMain.h"
#include "ofxKsmrStepManager.h"
#include "ofxKsmrParallellinkDelta.h"


class ParallelLinkManager{
public:
	
	void setup(string serial);
	void setupOsc(string host,int port){
		stepManager.setupOsc(host, port);
	};

	void update();
	void draw();

	void setPlot_inClamp(ofVec3f p);
	void calibrate();

	ofxKsmrStepManager			stepManager;
	ofxKsmrParallellinkDelta	delta;

	float radius;
	float armLength1;
	float armLength2;
	float height;
	float plot_radius;
	
	ofVec3f area_clamp;
	ofVec3f area_offset;

	bool	enableSync;
	int		id_offset;
};

#endif /* defined(__motorTester__ParallelLinkManager__) */
