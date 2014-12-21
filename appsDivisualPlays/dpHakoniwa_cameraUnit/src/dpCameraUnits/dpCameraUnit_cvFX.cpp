//
//  dpCameraUnit_cvFX.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_cvFX.h"

dpCameraUnit_cvFX::dpCameraUnit_cvFX(){

	mGui.setup();
	mGui.addLabel("cvEffect",OFX_UI_FONT_LARGE);
	mGui.addSpacer();
	mGui.addLabel("GrayChannel");
	mGui.addSpacer();
	mGui.addToggle("Blur", &mEnableBlur);
	mGui.addSlider("BlurSize", 0.0, 40.0, &mParam_Blur);
	mGui.addToggle("Threshold", &mEnableThreshold);
	mGui.addSlider("ThrVal", 0.0, 255.0, &mParam_Threshold);
	mGui.addToggle("Canny", &mEnableCanny);
	mGui.addToggle("FrameDiff", &mEnableFrameDiff);
	mGui.addRangeSlider("CannyThr", 0.0, 255.0, &mParam_Canny_Thr1, &mParam_Canny_Thr2);
	mGui.addToggle("Invert", &mEnableInvert);

	mGui.autoSizeToFitWidgets();

}

dpCameraUnit_cvFX::~dpCameraUnit_cvFX(){

}

void dpCameraUnit_cvFX::update(ofImage &pix, bool newFrame){
	
	if ((!mSource.isAllocated()) ||
		(pix.getWidth() != mSource.getWidth()) ||
		(pix.getHeight() != mSource.getHeight())){
		mSource.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_COLOR);
		mGraySource.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
		mGraySource_forDiff.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
	}

	if (mSource.isAllocated() && newFrame){
		ofxCv::copy(pix, mSource);
		ofxCv::convertColor(mSource, mGraySource, CV_RGB2GRAY);

		if (mEnableBlur)		ofxCv::blur(mGraySource, mGraySource, mParam_Blur);
		if (mEnableThreshold)	ofxCv::threshold(mGraySource, mGraySource, mParam_Threshold);
		if (mEnableInvert)		ofxCv::invert(mGraySource);
		if (mEnableCanny)		ofxCv::Canny(mGraySource, mGraySource, mParam_Canny_Thr2, mParam_Canny_Thr1);

		if (mEnableFrameDiff){
			ofxCv::absdiff(mGraySource, mGraySource_forDiff, mGraySource_forDiff);
			ofxCv::swap(mGraySource, mGraySource_forDiff);
		}

		mSource.update();
		mGraySource.update();
	}

}

void dpCameraUnit_cvFX::draw(int x,int y){

	mGui.setPosition(x, y);

	ofPushMatrix();
	ofTranslate(x, y);

	mSource.draw(240,0);
	mGraySource.draw(240, mSource.getHeight());

	ofPopMatrix();

}