//
//  ramMEXSync.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/19.
//
//

#include "ramMEXSync.h"

ramMEXSync::ramMEXSync(){

}

void ramMEXSync::setupControlPanel(){

	gui = ramGetGUI().getCurrentUIContext();
	targScene = gui->addTextInput("TargScene", "dpHPLink_Laser");
	targScene->setAutoClear(false);

	scenes.push_back("dpHStruggle");
	scenes.push_back("dpHPlink_Laser");
	gui->addRadio("sceneLs", scenes);

	gui->addButton("Sync", false);

	mex.setupControlPanel(this);

	ofAddListener(gui->newGUIEvent, this, &ramMEXSync::onPanelChanged);
	
	receiver.addAddress("/Debug/");
	ramOscManager::instance().addReceiverTag(&receiver);
}

void ramMEXSync::update(){

	mex.update();

	if (ofGetKeyPressed('1')){
		vector<string> st;
		st.push_back("kawaguchi");
		st.push_back("kojiri");
		st.push_back("sasamoto");
		mex.actorList->reshuffle(st);
	}

	if (ofGetKeyPressed('2')){
		mex.actorList->swapListItems(0, 1);
	}
}

void ramMEXSync::draw(){

	ramBeginCamera();
	mex.draw();
	ramEndCamera();

}

void ramMEXSync::onPanelChanged(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "Sync" && w->getState() == OFX_UI_STATE_DOWN){
		string addr = "/ram/MEX/"+targScene->getTextString();

		for (int i = 0;i < 2;i++){
			if (i == 0) sender.setup("192.168.20.2", 10000);
			if (i == 1) sender.setup("192.168.20.3", 10000);

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

			ofxOscMessage mLs;
			mLs.setAddress(addr+"/actorList");
			for (int i = 0;i < mex.actorList->getListItems().size();i++){
				mLs.addStringArg(mex.actorList->getListItems()[i]->getName());
			}
			sender.sendMessage(mLs);
		}

	}
}