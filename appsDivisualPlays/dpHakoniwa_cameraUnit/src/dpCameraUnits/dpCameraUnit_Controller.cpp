//
//  dpCameraUnit_Controller.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_Controller.h"

dpCameraUnit_Controller::dpCameraUnit_Controller(){

}

dpCameraUnit_Controller::~dpCameraUnit_Controller(){

}

void dpCameraUnit_Controller::update(){

	inputUnit	.update();
	shaderUnit	.update(inputUnit.mFinalSource_Large);
	cvFXUnit	.update(inputUnit.mFinalSource_Small, inputUnit.mIsFrameNew);
	cvAnalysis	.update(cvFXUnit.mSource, cvFXUnit.mGraySource);

}

void dpCameraUnit_Controller::draw(){

	inputUnit	.draw(0		, 0);
	shaderUnit	.draw(0		, 480);
	cvFXUnit	.draw(600	, 0);
	cvAnalysis	.draw(600	, 480);

}