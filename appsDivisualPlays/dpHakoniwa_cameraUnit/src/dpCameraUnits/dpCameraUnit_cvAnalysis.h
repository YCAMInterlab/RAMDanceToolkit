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
	bool mViewSource;
	
	bool mParamCF_Simplify;
	bool mParamCF_UseTargetColor;
	float mParamCF_MaxArea;
	float mParamCF_MinArea;
	float mParamCF_Threshold;
	ofFloatColor mParamCF_targColor;

	ofImage *imgRefColor, *imgRefGray;
	
	string hakoniwa_name;
	
	//Analysis Variables
	ofVec2f mOptFlow_angleVec;
	ofVec2f mOptFlow_sumVecs[10];
	ofVec2f mOptFlow_smoothVecs[10];
	float mOptFlow_filterSpd;
	float mOptFlowSmooth;

	vector <string> *oscListPtr;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpCameraUnit_cvAnalysis__) */
