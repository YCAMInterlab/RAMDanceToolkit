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

	mGui.addSpacer();
	mGui.addToggle("AdaptiveThreshold", &mEnableAdaptiveThreshold);
	mGui.addIntSlider("blockSize", 3, 255, &mParam_adpThreshold_blockSize);
	mGui.addIntSlider("offset", 3, 255, &mParam_adpThreshold_offset);
	mGui.addToggle("invert", &mParam_adpThreshold_invert);
	mGui.addToggle("gauss", &mParam_adpThreshold_gauss);

	mGui.addSpacer();
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
		if (mEnableAdaptiveThreshold) useAdaptiveThreshold(mGraySource,
														   mGraySource,
														   mParam_adpThreshold_blockSize,
														   mParam_adpThreshold_offset,
														   mParam_adpThreshold_invert,
														   mParam_adpThreshold_gauss);
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

	drawUI(x, y);
	drawThumbnail(x+240, y);

}

void dpCameraUnit_cvFX::drawUI(int x,int y){

	mGui.setPosition(x, y);

}

void dpCameraUnit_cvFX::drawThumbnail(int x, int y, float scale){

	ofPushMatrix();
	ofTranslate(x, y);
	glScaled(scale, scale, scale);

	ofSetColor(255);
	mSource.draw(0,0);
	mGraySource.draw(0, mSource.getHeight());

	ofPopMatrix();

}

void dpCameraUnit_cvFX::useAdaptiveThreshold(ofImage &src, ofImage &dst, int blockSize, int offset, bool invert, bool gauss){

	if(src.type != OF_IMAGE_GRAYSCALE){

		ofLogNotice("adaptiveThreshold(): src type must be OF_IMAGE_GRAYSCALE !!!");
		return;

	}

	if( blockSize < 2 ) {
		ofLogNotice("adaptiveThreshold(): block size") << blockSize << " < minimum, setting to 3";
		blockSize = 3;
	}

	if( blockSize % 2 == 0 ) {
		ofLogNotice() << "adaptiveThreshold(): block size " << blockSize << " not odd, adding 1";
		blockSize++;
	}

	int threshold_type = CV_THRESH_BINARY;
	if(invert) threshold_type = CV_THRESH_BINARY_INV;

	int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
	if(gauss) adaptive_method = CV_ADAPTIVE_THRESH_GAUSSIAN_C;

	ofxCv::adaptiveThreshold(ofxCv::toCv(src), tmp, 255,
							 adaptive_method, threshold_type, blockSize, offset);
	ofxCv::toOf(tmp,dst);

}