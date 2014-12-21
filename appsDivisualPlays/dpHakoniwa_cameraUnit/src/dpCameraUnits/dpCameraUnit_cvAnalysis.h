//
//  dpCameraUnit_cvAnalysis.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpCameraUnit_cvAnalysis__
#define __dpHakoniwa_cameraUnit__dpCameraUnit_cvAnalysis__

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "ofxOsc.h"

class dpCameraUnit_cvAnalysis{
public:
	dpCameraUnit_cvAnalysis();
	~dpCameraUnit_cvAnalysis();

	void update(ofImage &pixColor, ofImage &pixGray);
	void draw(int x,int y);

	ofxOscSender sender;
	ofxUICanvas mGui;

	ofxCv::ContourFinder contFinder;
	ofxCv::Scalar means;
	ofxCv::Scalar means_gray;

	bool mEnableSendOSC;
	bool mEnableContourFinder;
	bool mEnableFAST;
	bool mEnableMean;
	bool mEnableHistgram;

	bool mParamCF_Simplify;
	bool mParamCF_UseTargetColor;
	float mParamCF_MaxArea;
	float mParamCF_MinArea;
	float mParamCF_Threshold;
	ofFloatColor mParamCF_targColor;

	ofImage *imgRefColor, *imgRefGray;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_cvAnalysis__) */
