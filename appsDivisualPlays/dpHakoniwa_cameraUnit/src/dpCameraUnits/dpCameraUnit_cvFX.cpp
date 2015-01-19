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
	mGui.addToggle("BackGround",&mEnableBackground);
	mGui.addToggle("RunningBack", &mEnableRunningBackground);
	mGui.addSlider("LearnTime", 0.0, 2000.0, &mParam_RB_LearnTime)->setValue(900.0);
	mGui.addSlider("Threshold", 0.0, 255.0, &mParam_RB_Threshold)->setValue(10.0);
	mGui.addToggle("Warp", &mEnableWarpPerspective);
	mGui.addToggle("Blur", &mEnableBlur);
	mGui.addSlider("BlurSize", 0.0, 40.0, &mParam_Blur);
	mGui.addSpacer();
	mGui.addToggle("Invert", &mEnableInvert);
	mGui.addToggle("Canny", &mEnableCanny);
	mGui.addRangeSlider("CannyThr", 0.0, 255.0, &mParam_Canny_Thr1, &mParam_Canny_Thr2);
	mGui.addSpacer();
	mGui.addToggle("FrameDiff", &mEnableFrameDiff);
    mGui.addToggle("Threshold", &mEnableThreshold);
    mGui.addSlider("ThrVal", 0.0, 255.0, &mParam_Threshold);
	mGui.addToggle("AdaptiveThreshold", &mEnableAdaptiveThreshold);
	mGui.addIntSlider("blockSize", 3, 255, &mParam_adpThreshold_blockSize);
	mGui.addIntSlider("offset", 3, 255, &mParam_adpThreshold_offset);
	mGui.addToggle("invert", &mParam_adpThreshold_invert);
	mGui.addToggle("gauss", &mParam_adpThreshold_gauss);
	mGui.addSpacer();
	mGui.addToggle("AccumelateWeight", &mEnableAccumlateWeight);
	mGui.addSlider("Time", 0.0, 1.0, &mParam_accum_Time);
	mGui.addSpacer();
	mGui.addToggle("Dilate", &mEnableDilate);
	mGui.addIntSlider("NumProc", 0, 10, &mParam_Dilate_num);
	mGui.addSpacer();
	mGui.addToggle("Erode", &	mEnableErode);
	mGui.addIntSlider("NumProc", 0, 10, &mParam_Erode_num);


	mGui.autoSizeToFitWidgets();

	ofAddListener(ofEvents().mousePressed, this, &dpCameraUnit_cvFX::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &dpCameraUnit_cvFX::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &dpCameraUnit_cvFX::mouseReleased);

	mParam_Canny_Thr1 = 100;
	mParam_Canny_Thr2 = 120;
	mParam_Threshold = 128;
	mParam_adpThreshold_blockSize = 5;
	mParam_adpThreshold_offset = 10;
	mParam_accum_Time = 0.1;
	mParam_Dilate_num = 5;
	mParam_Erode_num = 5;

}

dpCameraUnit_cvFX::~dpCameraUnit_cvFX(){

}

void dpCameraUnit_cvFX::update(ofImage &pix, bool newFrame){
	
	if ((!mSource.isAllocated()) ||
		(pix.getWidth() != mSource.getWidth()) ||
		(pix.getHeight() != mSource.getHeight())){
		mSource.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_COLOR);
		mSourceTmp.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_COLOR);

		mGraySource.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
		mGraySource_background.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
		mGraySource_tmp.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_GRAYSCALE);
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

		if (mEnableRunningBackground){
			
//			backGround.setLearningRate(mParam_RB_LearnTime);
			backGround.setThresholdValue(10);
			backGround.update(mSource, mGraySource);
			swap(mSource, mSourceTmp);
			mSource.update();
//			ofxCv::convertColor(mSourceTmp, mGraySource, CV_RGB2GRAY);
		}else{

		}

		if (mEnableWarpPerspective){
			if (mResetWarpPt){
				mResetWarpPt = false;

				mUnwarpPts[0].set(0, 0);
				mUnwarpPts[1].set(mGraySource.getWidth(), 0);
				mUnwarpPts[2].set(mGraySource.getWidth(), mGraySource.getHeight());
				mUnwarpPts[3].set(0, mGraySource.getHeight());

			}

			vector<ofxCv::Point2f> warpPt;
			for (int i = 0;i < 4;i++) warpPt.push_back(ofxCv::toCv(mUnwarpPts[i]));
			ofxCv::copy(mGraySource, mGraySource_tmp);
			mGraySource_tmp.update();
			ofxCv::unwarpPerspective(mGraySource_tmp, mGraySource, warpPt);

		}else{
			mResetWarpPt = true;
		}

		if (mEnableBlur)		ofxCv::blur(mGraySource, mGraySource, mParam_Blur);
		if (mEnableInvert)		ofxCv::invert(mGraySource);
		if (mEnableCanny)		ofxCv::Canny(mGraySource, mGraySource, mParam_Canny_Thr2, mParam_Canny_Thr1);

		if (mEnableFrameDiff){
			ofxCv::absdiff(mGraySource, mGraySource_forDiff, mGraySource_forDiff);
			ofxCv::swap(mGraySource, mGraySource_forDiff);
		}
        if (mEnableThreshold)	ofxCv::threshold(mGraySource, mGraySource, mParam_Threshold);
		if (mEnableAdaptiveThreshold) useAdaptiveThreshold(mGraySource,
														   mGraySource,
														   mParam_adpThreshold_blockSize,
														   mParam_adpThreshold_offset,
														   mParam_adpThreshold_invert,
														   mParam_adpThreshold_gauss);
		
		if (mEnableAccumlateWeight) useAccumulateWeighted(&mGraySource,
														  &mGraySource,
														  &mAccum, mParam_accum_Time);

		if (mEnableDilate) {
			for (int i = 0;i < mParam_Dilate_num;i++){
				ofxCv::dilate(mGraySource);
			}
		}

		if (mEnableErode){
			for (int i = 0;i < mParam_Erode_num;i++){
				ofxCv::erode(mGraySource);
			}
		}

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
	mLastDrawPos.set(x,y);
	mLastDrawScale = scale;

	ofSetColor(255);
	mSource.draw(0,0);
	mGraySource.draw(0, mSource.getHeight());

	if (mEnableWarpPerspective){
		ofSetColor(255, 255, 0);
		glBegin(GL_LINE_LOOP);
		for (int i = 0;i < 4;i++){
			glVertex2d(mUnwarpPts[i].x, mUnwarpPts[i].y);
		}
		glEnd();
	}

	ofPopMatrix();

}

void dpCameraUnit_cvFX::mousePressed(ofMouseEventArgs &arg){
	ofVec2f mp;
	mp.set(arg.x, arg.y);
	mp -= mLastDrawPos;
	mp /= mLastDrawScale;

	mWarpTarget = -1;
	for (int i = 0;i < 4;i++){
		if (mp.distanceSquared(mUnwarpPts[i]) < 50){
			mWarpTarget = i;
		}
	}
}

void dpCameraUnit_cvFX::mouseDragged(ofMouseEventArgs &arg){

	ofVec2f mp;
	mp.set(arg.x, arg.y);
	mp -= mLastDrawPos;
	mp /= mLastDrawScale;

	if (mWarpTarget > -1){
		mUnwarpPts[mWarpTarget].set(mp);
	}

}

void dpCameraUnit_cvFX::mouseReleased(ofMouseEventArgs &arg){
	mWarpTarget = -1;
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

void dpCameraUnit_cvFX::savePreset(string hakoniwaName){
	if (hakoniwaName == "") return;

	ofDirectory::createDirectory("Preset_"+hakoniwaName);
	mGui.saveSettings("Preset_"+hakoniwaName+"/UIPreset.xml");

	ofxXmlSettings xml;
	for (int i = 0;i < 4;i++){
		xml.addValue("WARP_"+ofToString(i)+"X", mUnwarpPts[i].x);
		xml.addValue("WARP_"+ofToString(i)+"Y", mUnwarpPts[i].y);
	}

	xml.save("Preset_"+hakoniwaName+"/WarpPts.xml");
}

void dpCameraUnit_cvFX::loadPreset(string hakoniwaName){
	mGui.loadSettings("Preset_"+hakoniwaName+"/UIPreset.xml");
	ofxXmlSettings xml;
	xml.load("Preset_"+hakoniwaName+"/WarpPts.xml");

	for (int i = 0;i < 4;i++){
		mUnwarpPts[i].x = xml.getValue("WARP_"+ofToString(i)+"X", 0.0);
		mUnwarpPts[i].y = xml.getValue("WARP_"+ofToString(i)+"Y", 0.0);
	}
}