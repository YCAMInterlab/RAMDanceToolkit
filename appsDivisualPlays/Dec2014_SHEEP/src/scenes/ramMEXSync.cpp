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

	scenes.push_back("dpHServoPendulum");
	scenes.push_back("dpVisStage");
	scenes.push_back("dpHWorm");
	scenes.push_back("dpHfrozenIce");
	scenes.push_back("dpHStruggle");
	scenes.push_back("dpHSandStorm");
	scenes.push_back("dpHMagPendulum");
 	scenes.push_back("dpVisTheta");
	scenes.push_back("dpHGearMove");
	scenes.push_back("dpHTornado");
	scenes.push_back("dpHPLink_Laser");
	scenes.push_back("dpHPLink_Prism");
	scenes.push_back("dpHPLink_Oil");
	
	gui->addLabel("SceneSelect",OFX_UI_FONT_LARGE);
	gui->addRadio("SceneSelector", scenes);
	gui->addSpacer();
	gui->addLabel("SceneControl",OFX_UI_FONT_LARGE);
	gui->addToggle("Enable", &mSignal_Enable);
	gui->addToggle("Disp1", &mSignal_DispA);
	gui->addToggle("Disp2", &mSignal_DispB);
	gui->addLabel("SendSignal",OFX_UI_FONT_LARGE);
	gui->addButton("Send", false);
	
	gui->addSpacer();
	gui->addLabel("Extractor",OFX_UI_FONT_LARGE);
	gui->addButton("Sync", false);
	gui->addButton("Get", false);
	
	mex.setupControlPanel(this);

	ofAddListener(gui->newGUIEvent, this, &ramMEXSync::onPanelChanged);
	
	receiver.addAddress("/Debug/");
	ramOscManager::instance().addReceiverTag(&receiver);
	
}

void ramMEXSync::update(){

	mSignal_DispA = mSignal_DispA & mSignal_Enable;
	mSignal_DispB = mSignal_DispB & mSignal_Enable;
	
	while (receiver.hasWaitingMessages()){
		
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		cout << "=======Dump======" << endl;
		cout << m.getAddress() << endl;
		for (int i = 0;i < m.getNumArgs();i++){
			if (m.getArgType(i) == OFXOSC_TYPE_INT32) cout << m.getArgAsInt32(i) << endl;
			if (m.getArgType(i) == OFXOSC_TYPE_INT64) cout << m.getArgAsInt64(i) << endl;
			if (m.getArgType(i) == OFXOSC_TYPE_STRING) cout << m.getArgAsString(i) << endl;
			if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) cout << m.getArgAsFloat(i) << endl;
		}
		
	}
	
	mex.update();

}

void ramMEXSync::draw(){

	ramBeginCamera();
	mex.draw();
	ramEndCamera();

}

void ramMEXSync::onPanelChanged(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "Send" && w->getState() == OFX_UI_STATE_DOWN){
		for (int i = 0;i < 2;i++){
			if (i == 0) sender.setup(ip_1, 10000);
			if (i == 1) sender.setup(ip_2, 10000);
			ofxOscMessage setScene;
			setScene.setAddress("/ram/set_scene");
			setScene.addStringArg(targScene->getTextString());
			setScene.addIntArg(mSignal_Enable);
			setScene.addIntArg(mSignal_DispA);
			setScene.addIntArg(mSignal_DispB);
			
			sender.sendMessage(setScene);
		}
	}
	
	if (w->getName() == "SceneSelector"){
		ofxUIRadio* r = (ofxUIRadio*)(w);
		targScene->setTextString(r->getActiveName());
	}
	
	if (w->getName() == "Sync" && w->getState() == OFX_UI_STATE_DOWN){
		string addr = "/ram/MEX/"+targScene->getTextString();

		for (int i = 0;i < 2;i++){
			if (i == 0) sender.setup(ip_1, 10000);
			if (i == 1) sender.setup(ip_2, 10000);

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

	if (w->getName() == "Get" && w->getState() == OFX_UI_STATE_DOWN){

		for (int i = 0;i < 2;i++){

			if (i == 0) sender.setup(ip_1, 10000);
			if (i == 1) sender.setup(ip_2, 10000);

			ofxOscMessage req;
			req.setAddress("/ram/MEX/"+targScene->getTextString()+"/request");
			req.addStringArg(getName());

			sender.sendMessage(req);

		}

	}
}