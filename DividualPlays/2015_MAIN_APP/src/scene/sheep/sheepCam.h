//
//  sheepCam.h
//  2015_MAIN_APP
//
//  Created by Ovis aries on 2015/01/20.
//
//

#ifndef ___015_MAIN_APP__sheepCam__
#define ___015_MAIN_APP__sheepCam__

#include "ramMain.h"

enum sheepCam_Mode{
	SHEEPCAM_STAY,
	SHEEPCAM_SIGMOID_WANDER
};

class sheepCam{
public:

	sheepCam(){
		camera.setDistance(500);
		mSw_position.set(200,200,200);
		mSw_targetFrame = 0;
		mSw_beginFrame = 0;
	};

	~sheepCam(){};

	void update();
	void setMode(sheepCam_Mode mode);

	ofEasyCam camera;
	sheepCam_Mode curMode;

	//sigmoid_wander
	int			mSw_step;
	int			mSw_beginFrame;
	int			mSw_targetFrame;
	ofVec3f		mSw_positionBef;
	ofVec3f		mSw_position;
	ofVec3f		mSw_target;


	float sigmoid(float x,bool flip = false){
		x = (x - 0.6) * 20.0;
		float e = 2.71828182846;

		if (!flip) return 1.0 / (1.0 + pow(e,-x));
		else	   return 1.0 - (1.0 / (1.0 + pow(e, -x)));
	}
};

#endif /* defined(___015_MAIN_APP__sheepCam__) */
