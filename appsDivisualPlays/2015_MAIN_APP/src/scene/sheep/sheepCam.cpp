//
//  sheepCam.cpp
//  2015_MAIN_APP
//
//  Created by Ovis aries on 2015/01/20.
//
//

#include "sheepCam.h"

void sheepCam::update(){
	if (curMode == SHEEPCAM_SIGMOID_WANDER){
		int curFrm = ofGetFrameNum();

		cout << "====Sheep Cam=====" << endl;
		cout << "BEgin :" << mSw_beginFrame << endl;
		cout << "Targ  :" << mSw_targetFrame << endl;
		cout << "Curren:" << curFrm << endl;

		if ((mSw_beginFrame < curFrm) &&
			(curFrm < mSw_targetFrame)){

			float tgPct = (curFrm - mSw_beginFrame) / float(mSw_targetFrame - mSw_beginFrame);
			mSw_position = mSw_positionBef.interpolate(mSw_target,
													   sigmoid(tgPct));

		}

		if (curFrm > mSw_targetFrame){
			mSw_beginFrame = curFrm + ofRandom(300)+100;
			mSw_targetFrame = mSw_beginFrame + 1500;

			mSw_positionBef = mSw_position;
			mSw_target.set(ofRandomf()*500,
						   ofRandomuf()*300+100,
						   ofRandomf()*500);
		}
		camera.setPosition(mSw_position);
		camera.lookAt(ofVec3f(0,0,0));
	}
}

void sheepCam::setMode(sheepCam_Mode mode){
	curMode = mode;
}