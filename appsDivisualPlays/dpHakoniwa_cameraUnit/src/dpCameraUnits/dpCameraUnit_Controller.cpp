//
//  dpCameraUnit_Controller.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_Controller.h"

dpCameraUnit_Controller::dpCameraUnit_Controller(){

	for (int i = 0;i < 4;i++){
		cvAnalysis[i].mGui.setPosition(-1000, 0);
		cvFXUnit[i].mGui.setPosition(-1000, 0);
	}
//	cvAnalysis_second.mGui.setPosition(0, 480);
//	cvFXUnit_second.mGui.setPosition(1200, 0);

	cvAnalysis[0].hakoniwa_name = "prism";
	cvAnalysis[1].hakoniwa_name = "pendulum";
	cvAnalysis[2].hakoniwa_name = "struggle";
	cvAnalysis[3].hakoniwa_name = "hakoniwa_D";

//	simulator_initialize();
}

dpCameraUnit_Controller::~dpCameraUnit_Controller(){

}

void dpCameraUnit_Controller::update(){
	
	inputUnit	.update();
	
	if (inputUnit.mFourSplit){
		for (int i = 0;i < 4;i++){
			cvFXUnit[i]  .update(inputUnit.mFinalSource_FourSplit[i], inputUnit.mIsFrameNew);
			cvAnalysis[i].update(cvFXUnit[i].mSource, cvFXUnit[i].mGraySource, inputUnit.mIsFrameNew);
		}

	}else{
		cvFXUnit[0]		.update(inputUnit.mFinalSource_Small, inputUnit.mIsFrameNew);
		cvAnalysis[0]	.update(cvFXUnit[0].mSource, cvFXUnit[0].mGraySource, inputUnit.mIsFrameNew);
	}
	

}

void dpCameraUnit_Controller::draw(){

	inputUnit.drawUI(0, 0);
	inputUnit.drawThumbnail(0, 110, 0.5);

	if (inputUnit.mFourSplit){
		for (int i = 0;i < 4;i++){

			cvFXUnit[i].drawUI			(240 + i*370, 0);
			cvFXUnit[i].drawThumbnail	(450 + i*370, 0, 0.5);

			cvAnalysis[i].drawUI		(240 + i * 370, 500);
			cvAnalysis[i].drawThumbnail	(450 + i * 370, 500, 0.5);

		}
	}else{

		cvFXUnit[0].draw(240,0);
		cvAnalysis[0].draw(240,500);

	}
}

void dpCameraUnit_Controller::simulator_initialize(){
	for (int i = 0;i < inputUnit.mCameraList.size();i++){
		if (inputUnit.mCameraList[i] == "Video") inputUnit.mSourceType = i;
	}
	inputUnit.mVideoPlayer.play();
	inputUnit.mFourSplit = true;

//	cvFXUnit_second		.mEnableFrameDiff = true;
//
//
//	cvAnalysis			.mEnableOptFlow = true;
//	cvAnalysis_second	.mEnableOptFlow = true;
//
//	cvAnalysis			.mEnableSendOSC = true;
//	cvAnalysis_second	.mEnableSendOSC = true;
}