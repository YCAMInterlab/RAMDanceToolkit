//
//  dpCameraUnit_Controller.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_Controller.h"

dpCameraUnit_Controller::dpCameraUnit_Controller(){

	for (int i = 0;i < NUM_CV_UNIT;i++){
		cvAnalysis[i].mGui.setPosition(-1000, 0);
		cvAnalysis[i].oscListPtr = &oscSendList;
		cvAnalysis[i].pairFXUnit = &cvFXUnit[i];
		cvFXUnit[i].mGui.setPosition(-1000, 0);
		cvAnalysis[i].hakoniwa_name = "";
	}

	makeSettings_targetInput = 1;

}

dpCameraUnit_Controller::~dpCameraUnit_Controller(){

}

void dpCameraUnit_Controller::update(){

	static bool windowReShape;
	if (mbMakeSettings){
		if (mbMakeSettings != windowReShape) ofSetWindowShape(635, 1080);
	}else{
		if (mbMakeSettings != windowReShape) ofSetWindowShape(1920, 1080);
	}
	windowReShape = mbMakeSettings;

	inputUnit	.update();
	
	if (inputUnit.mFourSplit){
		if (mbMakeSettings){
			cvFXUnit[0].update(inputUnit.mFinalSource_FourSplit[makeSettings_targetInput],
							   inputUnit.mIsFrameNew);
			cvAnalysis[0].update(cvFXUnit[0].mSource,
								 cvFXUnit[0].mGraySource,
								 inputUnit.mIsFrameNew);
		}else{
			for (int i = 0;i < 4;i++){
				cvFXUnit[i]  .update(inputUnit.mFinalSource_FourSplit[i],
									 inputUnit.mIsFrameNew);
				cvAnalysis[i].update(cvFXUnit[i].mSource,
									 cvFXUnit[i].mGraySource,
									 inputUnit.mIsFrameNew);
			}
		}

	}else{
		cvFXUnit[0]		.update(inputUnit.mFinalSource_Small, inputUnit.mIsFrameNew);
		cvAnalysis[0]	.update(cvFXUnit[0].mSource, cvFXUnit[0].mGraySource, inputUnit.mIsFrameNew);
	}

}

void dpCameraUnit_Controller::draw(){

	inputUnit.drawUI(0, 10);
	inputUnit.drawThumbnail(0, 160, 0.5);

	if (inputUnit.mFourSplit){
		for (int i = 0;i < NUM_CV_UNIT;i++){

			ofSetColor(50 + 40 * (i % 2));
			ofRect(210 + i * 420, 0, 420, ofGetHeight());

			ofSetColor(255);
			cvFXUnit[i].drawUI			(210 + i*420, 0);
			cvFXUnit[i].drawThumbnail	(210 + i*420, ofGetHeight() - 300, 0.5);

			cvAnalysis[i].drawUI		(420 + i * 420, 0);
			cvAnalysis[i].drawThumbnail	(420 + i * 420, ofGetHeight() - 300, 0.5);


		}
	}else{

		cvFXUnit[0].draw(210,0);
		cvAnalysis[0].draw(420,0);

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

void dpCameraUnit_Controller::guiEvent(ofxUIEventArgs &e){
	
	ofxUIWidget* w = e.widget;

	if (w->getName().substr(0,4) == "OSC_"){
		refleshAddressList();
	}
	
	if (w->getName() == "Load Setting"){

	}
	
	if (w->getName() == "Save Setting"){
		inputUnit.mGui.saveSettings("inputUnit.xml");
		
		for (int i = 0;i < 4;i++){
			cvFXUnit[i]		.mGui.saveSettings("cvFxUnit_"+ofToString(i)+".xml");
			cvAnalysis[i]	.mGui.saveSettings("Analysis_"+ofToString(i)+".xml");
		}
	}

	if (w->getName().substr(0,10) == "LoadPreset"){
		ofxUIToggle* tm = (ofxUIToggle*)(e.widget);

		int i = ofToInt(w->getName().substr(11,1));
		int j = ofToInt(w->getName().substr(14,1));

		inputUnit.mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/inputUnit.xml");
		inputUnit.sourceReflesh();

		for (int o = 0;o < 4;o++){
			cvFXUnit[o].mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/cvFxUnit_"+ofToString(o)+".xml");
			cvAnalysis[o].mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/Analysis_"+ofToString(o)+".xml");
		}
		
	}
	
	if (w->getName().substr(0,10) == "SavePreset"){
		ofxUIToggle* tm = (ofxUIToggle*)(e.widget);

		int i = ofToInt(w->getName().substr(11,1));
		int j = ofToInt(w->getName().substr(14,1));

		ofDirectory::createDirectory("Preset"+ofToString(i)+ofToString(j));
		
		inputUnit.mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/inputUnit.xml");
		
		for (int o = 0;o < 4;o++){
			cvFXUnit[o].mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/cvFxUnit_"+ofToString(o)+".xml");
			cvAnalysis[o].mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/Analysis_"+ofToString(o)+".xml");
		}
		
	}
	
	if (w->getName() == "namePreset"){
		ofxUITextInput* ti = (ofxUITextInput*)(e.widget);
		ti->setTextString(ti->getTextString());
	}

	if (w->getName() == "SaveHakoPreset"){
		for (int i = 0; i < 4;i++){
			cvFXUnit[i]		.savePreset(cvAnalysis[i].hakoniwa_name);
			cvAnalysis[i]	.savePreset(cvAnalysis[i].hakoniwa_name);
		}
	}
}

void dpCameraUnit_Controller::refleshAddressList(){
	oscSendList.clear();
}
