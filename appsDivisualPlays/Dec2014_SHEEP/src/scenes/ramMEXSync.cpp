//
//  ramMEXSync.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/19.
//
//

#include "ramMEXSync.h"

void ramMEXSync::setupControlPanel(){

	gui = ramGetGUI().getCurrentUIContext();
	targScene = gui->addTextInput("TargScene", "");
	targScene->setAutoClear(false);

	scenes.push_back("dpHStruggle");
	scenes.push_back("dpHPlink_Laser");
	gui->addRadio("sceneLs", scenes);

	gui->addButton("Sync", false);

	mex.setupControlPanel(this);

}

void ramMEXSync::update(){

	mex.update();

}

void ramMEXSync::draw(){

	mex.draw();

}

void ramMEXSync::onPanelChanged(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "Sync"){
		string addr = "/ram/MEX/"+targScene->getTextString();
		sender.setup("192.168.20.1", 10000);

		ofxOscMessage mCls;
		mCls.setAddress(addr+"/clear");

		sender.sendMessage(mCls);

		for (int i = 0;i < mex.mMotionPort.size();i++){
			ofxOscMessage mPsh;
			mPsh.setAddress(addr+"/push");
			mPsh.addIntArg(mex.mMotionPort[i]->mActorIndex);
			mPsh.addIntArg(mex.mMotionPort[i]->mFinder.index);

			sender.sendMessage(mPsh);
		}
	}
}