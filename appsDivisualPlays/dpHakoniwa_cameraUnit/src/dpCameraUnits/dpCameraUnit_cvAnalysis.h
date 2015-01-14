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

	void update(ofImage &pixColor, ofImage &pixGray, bool isFrameNew);
	void draw(int x,int y);
	void drawUI(int x,int y);
	void drawThumbnail(int x,int y, float scale = 1.0);

	void savePreset(string hakoniwaName);
	void loadPreset(string hakoniwaName);

	void guiEvent(ofxUIEventArgs& ev);

	void sendMessageMulti(ofxOscMessage &m);

	ofxOscSender sender;
	ofxUICanvas mGui;
	ofxUITextInput* addUI;
	ofxUITextInput* portUI;
	ofxUIToggleMatrix* oscMatrixUI;

	ofxCv::ContourFinder	mContFinder;
	ofxCv::FlowPyrLK		mOptFlow;
	ofxCv::FlowFarneback	mOptFlowFarne;
	ofxCv::Scalar means;
	ofxCv::Scalar means_gray;

	bool mEnableSendOSC;
	bool mEnableContourFinder;
	bool mEnableOptFlow;
	bool mEnableOptFlowFarne;
	bool mEnableFAST;
	bool mEnableMean;
	bool mEnableHistgram;
	bool mEnablePixelate;
	bool mViewSource;
	
	bool mParamCF_Simplify;
	bool mParamCF_UseTargetColor;
	float mParamCF_MaxArea;
	float mParamCF_MinArea;
	float mParamCF_Threshold;
	float mParamCF_MaxBlobNum;
	int mParamPixelate_ResX;
	int mParamPixelate_ResY;
	ofFloatColor mParamCF_targColor;

	ofImage *imgRefColor, *imgRefGray;
	
	string hakoniwa_name;
	
	//Analysis Variables
	ofVec2f mOptFlow_angleVec;
	ofVec2f mOptFlow_sumVecs[10];
	ofVec2f mOptFlow_smoothVecs[10];
	float mOptFlow_filterSpd;
	float mOptFlowSmooth;
	vector <ofVec3f> mOpt_previous;

	vector <string> *oscListPtr;

	vector <bool> debug_px;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_cvAnalysis__) */
