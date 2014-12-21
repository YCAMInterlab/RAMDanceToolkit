//
//  dpCameraUnit_cvAnalysis.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_cvAnalysis.h"

dpCameraUnit_cvAnalysis::dpCameraUnit_cvAnalysis(){

	mGui.setup();
	mGui.addLabel("Analysis",OFX_UI_FONT_LARGE);
	mGui.addSpacer();

	mGui.addLabel("Switch");
	mGui.addToggle("SendOSC", &mEnableSendOSC);
	mGui.addToggle("ContourFinder", &mEnableContourFinder);
//	mGui.addToggle("FAST", &mEnableFAST);
	mGui.addToggle("Mean", &mEnableMean);
//	mGui.addToggle("Histgram", &mEnableHistgram);

	mGui.addLabel("ContourFinder");
	mGui.addToggle("Simplify"		, &mParamCF_Simplify);
	mGui.addToggle("UseTargetColor"	, &mParamCF_UseTargetColor);
	mGui.addRangeSlider("Area", 0.0, 5000.0, &mParamCF_MinArea, &mParamCF_MaxArea);
	mGui.addSlider("Threshold", 0.0, 255.0, &mParamCF_Threshold);

	imgRefColor = NULL;
	imgRefGray = NULL;

	mGui.autoSizeToFitWidgets();

	sender.setup("localhost", 12345);
}

dpCameraUnit_cvAnalysis::~dpCameraUnit_cvAnalysis(){

}

void dpCameraUnit_cvAnalysis::update(ofImage &pixColor, ofImage &pixGray){

	imgRefColor = &pixColor;
	imgRefGray = &pixGray;

	if (mEnableContourFinder){

		contFinder.setMaxArea(mParamCF_MaxArea);
		contFinder.setMinArea(mParamCF_MinArea);
		contFinder.setSimplify(mParamCF_Simplify);
		contFinder.setTargetColor(mParamCF_targColor);
		contFinder.setUseTargetColor(mParamCF_UseTargetColor);
		contFinder.setThreshold(mParamCF_Threshold);

		contFinder.findContours(pixGray);

		for (int i = 0;i < contFinder.getContours().size();i++){
			for (int j = 0;j < contFinder.getContour(i).size();j++){

				if ((j % 5 == 0) && (mEnableSendOSC)){
					ofVec2f pt = ofxCv::toOf(contFinder.getContour(i)[j]);
					ofxOscMessage m;
					m.setAddress("/dp/cameraUnit/contour");
					m.addIntArg(i);
					m.addIntArg(j);
					m.addFloatArg(pt.x);
					m.addFloatArg(pt.y);
					sender.sendMessage(m);
				}

			}
		}

	}
	if (mEnableMean){
		means = ofxCv::mean(ofxCv::toCv(pixColor));
		means_gray = ofxCv::mean(ofxCv::toCv(pixGray));

		if (mEnableSendOSC){
			ofxOscMessage m;
			m.setAddress("/dp/cameraUnit/mean");
			m.addIntArg(means[0]);
			m.addIntArg(means[1]);
			m.addIntArg(means[2]);
			m.addIntArg(means_gray[0]);
			sender.sendMessage(m);
		}
	}

}

void dpCameraUnit_cvAnalysis::draw(int x,int y){
	mGui.setPosition(x, y);
	ofPushMatrix();
	ofTranslate(x, y);

	ofTranslate(240,0);
	if (imgRefGray != NULL) imgRefGray->draw(0,0);
	ofSetColor(255, 255, 0);
	contFinder.draw();

	ofSetColor(255, 0, 0);
	ofRect(0, imgRefGray->getHeight()     , means[0], 10);
	ofSetColor(0, 255, 0);
	ofRect(0, imgRefGray->getHeight() + 10, means[1], 10);
	ofSetColor(0, 0, 255);
	ofRect(0, imgRefGray->getHeight() + 20, means[2], 10);
	ofSetColor(255);
	ofRect(0, imgRefGray->getHeight() + 30, means_gray[0], 10);

	ofPopMatrix();

	ofSetColor(255);
}