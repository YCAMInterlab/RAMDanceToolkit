//
//  dpCameraUnit_cvFX.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpCameraUnit_cvFX__
#define __dpHakoniwa_cameraUnit__dpCameraUnit_cvFX__

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"

class dpCameraUnit_cvFX{
public:
	dpCameraUnit_cvFX();
	~dpCameraUnit_cvFX();

	void update(ofImage &pix,bool NewFrame);
	void draw(int x,int y);

	ofxUICanvas mGui;
	ofImage mSource;
	ofImage mGraySource;
	ofImage mGraySource_forDiff;

	bool mEnableBlur;
	bool mEnableCanny;
	bool mEnableThreshold;
	bool mEnableInvert;
	bool mEnableFrameDiff;

	float mParam_Blur;
	float mParam_Canny_Thr1;
	float mParam_Canny_Thr2;
	float mParam_Threshold;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_cvFX__) */
