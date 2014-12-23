//
//  dpCameraUnit_Controller.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_Controller.h"

dpCameraUnit_Controller::dpCameraUnit_Controller(){
	cvAnalysis_second.mGui.setPosition(0, 480);
	cvFXUnit_second.mGui.setPosition(1200, 0);
	
	cvAnalysis.hakoniwa_name = "sand";
	cvAnalysis_second.hakoniwa_name = "pendulum";
	
	//for evala
	for (int i = 0;i < inputUnit.mCameraList.size();i++){
		if (inputUnit.mCameraList[i] == "Video") inputUnit.mSourceType = i;
	}
	inputUnit.mVideoPlayer.play();
	inputUnit.mFourSplit = true;
	
	cvFXUnit_second		.mEnableFrameDiff = true;
	
	
	cvAnalysis			.mEnableOptFlow = true;
	cvAnalysis_second	.mEnableOptFlow = true;
	
	cvAnalysis			.mEnableSendOSC = true;
	cvAnalysis_second	.mEnableSendOSC = true;
}

dpCameraUnit_Controller::~dpCameraUnit_Controller(){

}

void dpCameraUnit_Controller::update(){
	
	inputUnit	.update();
	
	if (inputUnit.mFourSplit){
		cvFXUnit	.update(inputUnit.mFinalSource_FourSplit[3], inputUnit.mIsFrameNew);
		cvFXUnit_second.update(inputUnit.mFinalSource_FourSplit[1], inputUnit.mIsFrameNew);
		
		cvAnalysis	.update(cvFXUnit.mSource, cvFXUnit.mGraySource);
		cvAnalysis_second.update(cvFXUnit_second.mSource, cvFXUnit_second.mGraySource);
		
	}else{
		cvFXUnit	.update(inputUnit.mFinalSource_Small, inputUnit.mIsFrameNew);
		cvAnalysis	.update(cvFXUnit.mSource, cvFXUnit.mGraySource);
	}
	

}

void dpCameraUnit_Controller::draw(){

	inputUnit	.draw(0		, 0);
	cvFXUnit	.draw(600	, 0);
	cvAnalysis	.draw(600	, 480);

	if (inputUnit.mFourSplit){
		cvAnalysis_second.draw(1200, 480);
		cvFXUnit_second.draw(1200, 0);
	}
}