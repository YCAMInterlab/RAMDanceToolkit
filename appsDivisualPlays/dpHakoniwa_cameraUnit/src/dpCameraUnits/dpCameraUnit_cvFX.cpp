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
//	mGui.addLabel("GrayChannel");
//	mGui.addSpacer();
	mGui.addToggle("Warp", &mEnableWarpPerspective);
	mGui.addToggle("Blur", &mEnableBlur);
	mGui.addSlider("BlurSize", 0.0, 40.0, &mParam_Blur);
	mGui.addSpacer();
	mGui.addToggle("AccumelateWeight", &mEnableAccumlateWeight);
	mGui.addSlider("Time", 0.0, 1.0, &mParam_accum_Time);
	mGui.addSpacer();
	mGui.addToggle("AdaptiveThreshold", &mEnableAdaptiveThreshold);
	mGui.addIntSlider("blockSize", 3, 255, &mParam_adpThreshold_blockSize);
	mGui.addIntSlider("offset", 3, 255, &mParam_adpThreshold_offset);
	mGui.addToggle("invert", &mParam_adpThreshold_invert);
	mGui.addToggle("gauss", &mParam_adpThreshold_gauss);
	mGui.addToggle("BackGround",&mEnableBackground);

	mGui.addSpacer();
//	mGui.addToggle("Threshold", &mEnableThreshold);
//	mGui.addSlider("ThrVal", 0.0, 255.0, &mParam_Threshold);
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
		mGraySource_background.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
		mGraySource_forDiff.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
	}

	if (mSource.isAllocated() && newFrame){
		ofxCv::copy(pix, mSource);
		ofxCv::convertColor(mSource, mGraySource, CV_RGB2GRAY);

		if (mEnableBackground){
			if (mBackgroundNeedsReflesh){
				mBackgroundNeedsReflesh = false;
				ofxCv::copy(mGraySource, mGraySource_background);
				mGraySource_background.update();
			}else{
				ofxCv::absdiff(mGraySource, mGraySource_background, mGraySource);
			}
			
		}else{
			mBackgroundNeedsReflesh = true;
		}

		if (mEnableWarpPerspective){
			if (mResetWarpPt){
				mResetWarpPt = false;

				mUnwarpPts[0].set(0.0, 0.0);
				mUnwarpPts[1].set(mGraySource.getWidth(), 0.0);
				mUnwarpPts[2].set(mGraySource.getWidth(), mGraySource.getHeight());
				mUnwarpPts[3].set(0.0, mGraySource.getHeight());

			}

			vector<ofxCv::Point2f> warpPt;
			for (int i = 0;i < 4;i++) warpPt.push_back(ofxCv::toCv(mUnwarpPts[i]));
			ofxCv::unwarpPerspective(mGraySource, tmp, warpPt);

		}else{
			mResetWarpPt = true;
		}

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
		if (mEnableAccumlateWeight) useAccumulateWeighted(&mGraySource,
														  &mGraySource,
														  &mAccum, mParam_accum_Time);

		mSource.update();
		mGraySource.update();
	}

}

void dpCameraUnit_cvFX::draw(int x,int y){

	drawUI(x, y);
	drawThumbnail(x, ofGetHeight() - 300);

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

	ofSetColor(255, 255, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0;i < 4;i++){
		glVertex2d(mUnwarpPts[i].x, mUnwarpPts[i].y);
	}
	glEnd();

	ofPopMatrix();

}

void dpCameraUnit_cvFX::useAccumulateWeighted(ofImage *src,ofImage *result,cv::Mat *accum,float time,bool fadeLastFrame){

	if(accum->empty())ofxCv::toCv(*src).convertTo(*accum,CV_32F); //最初に１フレーム格納が必要

	time = fmaxf(0.0, time);
	time = fminf(1.0,time);

	accumulateWeighted(ofxCv::toCv(*src),*accum, time);

	accum->convertTo(tmp, CV_8U);
	ofxCv::toOf(tmp,*result);

	if( !fadeLastFrame )ofxCv::add(*src, *result, *result); //srcと結果を加算。これをしないと最後に入ってきたフレームも移動平均で薄くなる
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