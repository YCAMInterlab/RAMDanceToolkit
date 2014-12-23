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
	mGui.addToggle("SendOSC",		&mEnableSendOSC);
	mGui.addTextInput("OSCAddress", "localhost")->setAutoClear(false);
	mGui.addTextInput("OSCPort", "10012")->setAutoClear(false);
	mGui.addToggle("ContourFinder", &mEnableContourFinder);
	mGui.addToggle("OptFlow",		&mEnableOptFlow);
	mGui.addToggle("Mean",			&mEnableMean);
//	mGui.addToggle("FAST",			&mEnableFAST);
//	mGui.addToggle("Histgram",		&mEnableHistgram);
	mGui.addSpacer();
	mGui.addToggle("ViewSource", &mViewSource);

	mGui.addLabel("ContourFinder");
	mGui.addSpacer();
	mGui.addToggle("Simplify"		, &mParamCF_Simplify);
	mGui.addToggle("UseTargetColor"	, &mParamCF_UseTargetColor);
	mGui.addRangeSlider("Area", 0.0, 10000.0, &mParamCF_MinArea, &mParamCF_MaxArea);
	mGui.addSlider("Threshold", 0.0, 255.0, &mParamCF_Threshold);

	ofAddListener(mGui.newGUIEvent, this, &dpCameraUnit_cvAnalysis::guiEvent);
	
	imgRefColor = NULL;
	imgRefGray = NULL;

	mGui.autoSizeToFitWidgets();

	sender.setup("localhost", 10012);
	
	mEnableSendOSC			= false;
	mEnableContourFinder	= false;
	mEnableOptFlow			= false;
	mEnableFAST				= false;
	mEnableMean				= false;
	mEnableHistgram			= false;
	
}

dpCameraUnit_cvAnalysis::~dpCameraUnit_cvAnalysis(){

}

void dpCameraUnit_cvAnalysis::update(ofImage &pixColor, ofImage &pixGray){

	imgRefColor = &pixColor;
	imgRefGray = &pixGray;

	if (mEnableContourFinder){
		mContFinder.setMaxArea(mParamCF_MaxArea);
		mContFinder.setMinArea(mParamCF_MinArea);
		mContFinder.setSimplify(mParamCF_Simplify);
		mContFinder.setTargetColor(mParamCF_targColor);
		mContFinder.setUseTargetColor(mParamCF_UseTargetColor);
		mContFinder.setThreshold(mParamCF_Threshold);

		mContFinder.findContours(pixGray);

		for (int i = 0;i < mContFinder.getContours().size();i++){
			for (int j = 0;j < mContFinder.getContour(i).size();j++){

				if ((j % 5 == 0) && (mEnableSendOSC)){
					ofVec2f pt = ofxCv::toOf(mContFinder.getContour(i)[j]);
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
	if (mEnableOptFlow){
		if (ofxCv::mean(ofxCv::toCv(pixGray))[0] > 1.0f)
			mOptFlow.calcOpticalFlow(pixGray);
		if ((ofGetFrameNum() % 300 == 0) ||
			(ofGetKeyPressed(' '))) mOptFlow.resetFlow();
		
		vector <ofVec2f> mot = mOptFlow.getMotion();
		
		for (int i = 0;i < 10;i++) mOptFlow_sumVecs[i].set(0.0,0.0);
		for (int i = 0;i < mot.size();i++){
			mOptFlow_sumVecs[i % 10] += mot[i];
		}
		for (int i = 0;i < 10;i++){
			mOptFlow_smoothVecs[i] += (mOptFlow_sumVecs[i] - mOptFlow_smoothVecs[i]) / 5.0;
		}
		
		if (mEnableSendOSC){
			for (int i = 0;i < 10;i++){
				ofxOscMessage m;
				m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/"+ofToString(i));
				m.addFloatArg(MIN(mOptFlow_smoothVecs[i].x,50.0));
				m.addFloatArg(MIN(mOptFlow_smoothVecs[i].y,50.0));
				sender.sendMessage(m);
			}
			for (int i = 0;i < 10;i++){
				ofxOscMessage m;
				m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/length/"+ofToString(i));
				m.addFloatArg(MIN(mOptFlow_smoothVecs[i].length(),50.0));
				sender.sendMessage(m);
			}
		}
	}

}

void dpCameraUnit_cvAnalysis::draw(int x,int y){
	mGui.setPosition(x, y);
	ofPushMatrix();
	ofTranslate(x, y);
	ofDrawBitmapString(hakoniwa_name, 0,400);

	ofTranslate(240,0);
	if (mViewSource && imgRefGray != NULL) imgRefGray->draw(0,0);
	ofSetColor(255, 255, 0);
	mContFinder.draw();
	mOptFlow.draw();

	if (mEnableOptFlow){
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(160, 120);
		for (int i = 0;i < 10;i++){
			ofLine(0, 0, mOptFlow_smoothVecs[i].x, mOptFlow_smoothVecs[i].y);
			ofCircle(mOptFlow_smoothVecs[i], 3);
			ofLine(i*20, 0,i*20,mOptFlow_smoothVecs[i].length());
		}
		ofPopMatrix();
	}
	
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

void dpCameraUnit_cvAnalysis::guiEvent(ofxUIEventArgs &ev){
	ofxUIWidget* w = ev.widget;
	
	if (w->getName() == "OSCAddress"){
		ofxUITextInput* addUI = (ofxUITextInput*)mGui.getWidget("OSCAddress");
		ofxUITextInput* portUI = (ofxUITextInput*)mGui.getWidget("OSCPort");
		if (addUI->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER){
			
			int pt = ofToInt(portUI->getTextString());
			string address = addUI->getTextString();
			
			sender.setup(address, pt);
			
			cout << "Sender setuped :" << address << " : " << pt << endl;
		}
	}
	if (w->getName() == "OSCPort"){
		ofxUITextInput* addUI = (ofxUITextInput*)mGui.getWidget("OSCAddress");
		ofxUITextInput* portUI = (ofxUITextInput*)mGui.getWidget("OSCPort");
		if (portUI->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER){
			
			int pt = ofToInt(portUI->getTextString());
			string address = addUI->getTextString();
			
			sender.setup(address, pt);
			
			cout << "Sender setuped :" << address << " : " << pt << endl;
		}
	}
	
}