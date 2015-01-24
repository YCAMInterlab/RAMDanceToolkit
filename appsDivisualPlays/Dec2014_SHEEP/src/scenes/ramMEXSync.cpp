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

	lastChanged = "";
	
	gui = ramGetGUI().getCurrentUIContext();
	targScene = gui->addTextInput("TargScene", "dpHPLink_Laser");
	targScene->setAutoClear(false);
	
	actorPreset.push_back("Standard");
	actorPreset.push_back("Solo");
	actorPreset.push_back("Duo");
	actorPreset.push_back("Trio");
	actorPreset.push_back("Preset_A");
	actorPreset.push_back("Preset_B");
	actorPreset.push_back("Preset_C");
	actorPreset.push_back("Preset_D");
	actorPreset.push_back("Preset_E");
	
	presetName.push_back("ServoPendulum");
	presetName.push_back("Stage");
	presetName.push_back("Worm");
	presetName.push_back("Ice");
	presetName.push_back("Struggle");
	presetName.push_back("SandStorm");
	presetName.push_back("MagPendulum");
	presetName.push_back("Theta");
	presetName.push_back("Gear");
	presetName.push_back("Tornado");
	presetName.push_back("Plink_Laser");
	presetName.push_back("Plink_Prism");
	presetName.push_back("Plink_Oil");
	presetName.push_back("EyeBallDancer");
	presetName.push_back("Metaball");
	presetName.push_back("OnNote");
	presetName.push_back("Kioku");

	enabled.assign(presetName.size(), false);
	
	scenes.push_back("dpHServoPendulum");	scenes_pair.push_back("dpVisServoPendulum");
	scenes.push_back("dpVisStage");			scenes_pair.push_back("");
	scenes.push_back("dpHWorm");			scenes_pair.push_back("dpVisWorm");
	scenes.push_back("dpHfrozenIce");		scenes_pair.push_back("dpVisIce");
	scenes.push_back("dpHStruggle");		scenes_pair.push_back("dpVisStruggle");
	scenes.push_back("dpHSandStorm");		scenes_pair.push_back("dpVisSandStorm");
	scenes.push_back("dpHMagPendulum");		scenes_pair.push_back("dpVisMagPendulum");
 	scenes.push_back("dpVisTheta");			scenes_pair.push_back("");
	scenes.push_back("dpHGearMove");		scenes_pair.push_back("");
	scenes.push_back("dpHTornado");			scenes_pair.push_back("dpVisTornado");
	scenes.push_back("dpHPLink_Laser");		scenes_pair.push_back("dpVisPLink_Laser");
	scenes.push_back("dpHPLink_Prism");		scenes_pair.push_back("dpVisPLink_Prism");
	scenes.push_back("dpHPLink_Oil");		scenes_pair.push_back("dpVisPLink_Oil");
	scenes.push_back("distanceMetaball");	scenes_pair.push_back("");
	scenes.push_back("OnNote");				scenes_pair.push_back("");
	scenes.push_back("Kioku");				scenes_pair.push_back("");
	scenes.push_back("dpHEyeBallDancer");
	
	gui->addLabel("SceneSelect",OFX_UI_FONT_LARGE);
	sceneRadio = gui->addRadio("SceneSelector", scenes);
	gui->addSpacer();
	gui->addLabel("SceneControl",OFX_UI_FONT_LARGE);
	gui->addToggle("Enable", &mSignal_Enable);
	gui->addToggle("Disp1", &mSignal_DispA);
	gui->addToggle("Disp2", &mSignal_DispB);
	gui->addLabel("SendSignal",OFX_UI_FONT_LARGE);
	gui->addButton("Send", false);
	gui->addButton("AllClear", false);
	
	gui->addSpacer();
	gui->addLabel("Extractor",OFX_UI_FONT_LARGE);
	gui->addButton("Sync", false);
	gui->addButton("Get", false);
	gui->addToggle("viewSimple", &mViewSimple);
	gui->addSpacer();
	gui->addLabel("Remote UI",OFX_UI_FONT_LARGE);
	uiName = gui->addTextInput("uiName", "UIName");
	uiName->setAutoClear(false);
	gui->addSlider("Remote:Float", 0.0, 1.0, &mUIRemote_float);
	gui->addSlider("Order", 1.0, 1000.0, &mUIRemote_fOrder);
	gui->addToggle("Remote:Toggle", &mUIRemote_toggle);
	
	presetGui = new ofxUICanvas();
	presetGui->disableAppDrawCallback();
	presetGui->disableMouseEventCallbacks();
	
	actPresetRadio = presetGui->addRadio("actorPreset", actorPreset);
	presetGui->addSpacer();
	presetGui->addButton("saveActPreset", false);
	presetGui->addToggle("RemotePreseting", &mRemotePreset);
	presetGui->autoSizeToFitWidgets();
	presetGui->setPosition(240, 400);
	presetGui->setup();

	blockLauncher = new ofxUICanvas();
	blockLauncher->disableAppDrawCallback();
	blockLauncher->disableMouseEventCallbacks();
	
	blockLauncher->setup();
	blockLauncher->addButton("Block_1:WormON to Kawaguchi", false);
	blockLauncher->addButton("Block_2:SandStormON to Kojiri/Sasa", false);
	blockLauncher->addButton("Block_3:SandStorm to 3Players WormOFF", false);
	blockLauncher->addButton("Block_4:SandStorm to Kawaguchi/Sasamoto", false);
	blockLauncher->addButton("Block_5:HakoniwaON Standby", false);
	blockLauncher->addButton("Block_6:Disable Vis", false);
	blockLauncher->addButton("Block_7:SandStorm without Vis to Kojiri/Sasa", false);
	blockLauncher->addButton("Block_8:SandStorm without Vis to Kawaguchi/Sasa", false);
	blockLauncher->addButton("Block_9:SandStorm without Vis to 3Players", false);
	blockLauncher->setPosition(240, 690);
	blockLauncher->autoSizeToFitWidgets();
	
	gui->addWidget(presetGui);
	gui->addWidget(blockLauncher);
	gui->autoSizeToFitWidgets();
	
	mex.setupControlPanel(this);

	ofAddListener(gui->newGUIEvent, this, &ramMEXSync::onPanelChanged);
	ofAddListener(presetGui->newGUIEvent, this, &ramMEXSync::onPanelChanged);
	ofAddListener(blockLauncher->newGUIEvent, this, &ramMEXSync::blockLaunch);
	
	receiver.addAddress("/Debug/");
	receiver.addAddress("/dp/cameraUnit/sceneState/");
	receiver.addAddress("/dp/cameraUnit/display");
	receiver.addAddress("/dp/system/remoteManage");
	ramOscManager::instance().addReceiverTag(&receiver);
	
	mRemotePreset = true;
	getterDelay = 0;
	
	switcher.setup();
	switcher.selector_hakoniwa->setVisible(false);
	switcher.selector_display->setVisible(false);
	
	mViewSimple = false;
}

void ramMEXSync::update(){
	
	switcher.update();
	
	getterDelay++;
	if (getterDelay == 40){
		if (mRemotePreset){
			
			string sceneName = sceneRadio->getActiveName();
			string presetName = actPresetRadio->getActiveName();
			
			mex.load("presentor/"+sceneName+"_"+presetName+".xml");
//			cout << mex.getNumPort() << endl;
			setExtractor();
			
		}else{
			actPresetRadio->activateToggle("");
			getExtractor();
		}
	}
	
	mSignal_DispA = mSignal_DispA & mSignal_Enable;
	mSignal_DispB = mSignal_DispB & mSignal_Enable;
	
	while (receiver.hasWaitingMessages()){
		
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if (m.getAddress().substr(0,26) == "/dp/cameraUnit/sceneState/"){
			
			string sceneName = m.getAddress().substr(26);

			int targId = -1;
			for (int i = 0;i < presetName.size();i++){
				if (presetName[i] == sceneName) targId = i;
			}
			
			if (targId > -1){
				
				if (!enabled[targId] && m.getArgAsInt32(0) > 0){
					ofxUIRadio* rd = (ofxUIRadio*)(gui->getWidget("SceneSelector"));
					rd->activateToggle(scenes[targId]);
					targScene->setTextString(rd->getActiveName());
					lastChanged = scenes[targId];
					getterDelay = 0;
				}
				
				enabled[targId] = m.getArgAsInt32(0);
				
			}
			
		}
		
		if (m.getAddress() == "/dp/cameraUnit/display"){
			currentScene[m.getArgAsInt32(0)] = m.getArgAsString(1);
		}
		
		if (m.getAddress().substr(0,7) == "/Debug/"){
			int idx = -1;
			for (int i = 0;i < previews.size();i++){
				if (previews[i]->msg.getAddress() == m.getAddress()){
					idx = i;
					break;
				}
			}
			
			if (idx == -1){
				
				oscPreview* pv = new oscPreview();
				pv->msg = m;
				pv->valueClamp.assign(pv->msg.getNumArgs(), 0.0);
				pv->numValue.assign(pv->msg.getNumArgs(), 0.0);
				
				for (int i = 0;i < m.getNumArgs();i++){
					if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
						pv->numValue[i] = m.getArgAsFloat(i);
					else if (m.getArgType(i) == OFXOSC_TYPE_INT32)
						pv->numValue[i] = m.getArgAsInt32(i);
					else if (m.getArgType(i) == OFXOSC_TYPE_INT64)
						pv->numValue[i] = m.getArgAsInt64(i);
				}
				
				previews.push_back(pv);
				
				
			}else{
				
				for (int i = 0;i < previews[idx]->msg.getNumArgs();i++){
					float v;
					if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) v = m.getArgAsFloat(i);
					else if (m.getArgType(i) == OFXOSC_TYPE_INT32) v = m.getArgAsInt32(i);
					else if (m.getArgType(i) == OFXOSC_TYPE_INT64) v = m.getArgAsInt64(i);
					else{
						v = 0.0;
					}
					previews[idx]->numValue[i] = v;
					previews[idx]->valueClamp[i] = MAX(previews[idx]->valueClamp[i],v);
				}
				
				previews[idx]->msg = m;
				
			}
			
			while (previews.size() > 8){
				oscPreview* p = previews[0];
				previews.erase(previews.begin());
				delete p;
			}
		}
	}
	
	mex.update();
}

void ramMEXSync::draw(){
	
	ramBeginCamera();
	mex.draw();
	ramEndCamera();
	
	if (mViewSimple) return;
	
	ofSetColor(dpColor::MAIN_COLOR);
	if ((0 < getterDelay) &&
		(getterDelay < 3)){
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
	}
	
	if ((6 < getterDelay) &&
		(getterDelay < 9)){
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
	}
	ofSetColor(255);
	
	
	drawDump();
	
	for (int i = 0;i < previews.size();i++){
		ofPushMatrix();
		ofTranslate(1100, i*130);
		
		ofNoFill();
		ofRect(0, 10, 300, 115);
		ofFill();
		
		string info = previews[i]->msg.getAddress() + "\n";
		info += previews[i]->msg.getRemoteIp() + "\n";
		ofSetColor(130,30,80);
		for (int j = 0;j < previews[i]->msg.getNumArgs();j++){
			if (previews[i]->msg.getArgType(j) == OFXOSC_TYPE_STRING)
				info += previews[i]->msg.getArgAsString(j) + "\n";
			else
				info += ofToString(previews[i]->numValue[j]) + "\n";
			ofRect(0, 30+j*13, 300 * previews[i]->numValue[j] / previews[i]->valueClamp[j], 8);
		}
		
		ofSetColor(255);
		ofDrawBitmapString(info, 10, 25);
		
		ofPopMatrix();
	}
	
	ofDisableDepthTest();
	for (int i = 0;i < 4;i++){
		ofPushMatrix();
		ofTranslate(1300 + (((i+1) % 4) % 2) * 300,
					600 + (((i+1) % 4) / 2) * 100);
		
		if (currentScene[i] == lastChanged){
			ofSetColor(dpColor::MAIN_COLOR * MAX(0,(255 - getterDelay) / 255.0));
			ofRect(0, 0, 280, 90);
		}
		
		ofSetColor(255);
		ofNoFill();
		ofRect(0, 0, 280, 90);
		ofFill();
		ofDrawBitmapString(currentScene[i], 30,30);
		
		
		ofPopMatrix();
	}
	ofEnableDepthTest();
}

void ramMEXSync::onPanelChanged(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "AllClear"){
		for (int i = 0;i < scenes.size();i++){
			ofxOscMessage scn,pair;
			scn.setAddress("/ram/set_scene");
			scn.addStringArg(scenes[i]);
			scn.addIntArg(0);
			scn.addIntArg(0);
			scn.addIntArg(0);

			pair.setAddress("/ram/set_scene");
			pair.addStringArg(scenes_pair[i]);
			pair.addIntArg(0);
			pair.addIntArg(0);
			pair.addIntArg(0);

			for (int j = 0;j < 2;j++){
				sender.setup(j == 0 ? ip_1 : ip_2, 10000);
				sender.sendMessage(scn);
				sender.sendMessage(pair);
			}
		}
	}

	if (w->getName() == "Remote:Float"){
		for (int i = 0;i < 2;i++){
			if (i == 0) sender.setup(ip_1, 10000);
			if (i == 1) sender.setup(ip_2, 10000);
			ofxOscMessage msg;
			msg.setAddress("/ram/MEX/"+targScene->getTextString()+"/UI");
			msg.addStringArg(uiName->getTextString());
			msg.addFloatArg(mUIRemote_float * mUIRemote_fOrder);
			
			sender.sendMessage(msg);
		}
	}
	
	if (w->getName() == "Remote:Toggle"){
		for (int i = 0;i < 2;i++){
			if (i == 0) sender.setup(ip_1, 10000);
			if (i == 1) sender.setup(ip_2, 10000);
			ofxOscMessage msg;
			msg.setAddress("/ram/MEX/"+targScene->getTextString()+"/UI");
			msg.addStringArg(uiName->getTextString());
			msg.addFloatArg(((ofxUIToggle*)(w))->getValue());
			
			sender.sendMessage(msg);
		}
	}

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

			int pId = -1;
			for (int j = 0;j < scenes.size();j++){
				if (targScene->getTextString() == scenes[j]){
					pId = j;
					break;
				}
			}

			if (pId != -1){
				ofxOscMessage pairScene;
				pairScene.setAddress("/ram/set_scene");
				pairScene.addStringArg(scenes_pair[pId]);
				pairScene.addIntArg(mSignal_Enable);
				pairScene.addIntArg(mSignal_DispA);
				pairScene.addIntArg(mSignal_DispB);

				sender.sendMessage(pairScene);
			}
		}
	}
	
	if (w->getName() == "SceneSelector"){
		ofxUIRadio* r = (ofxUIRadio*)(w);
		targScene->setTextString(r->getActiveName());
	}
	
	if (w->getName() == "Sync" && w->getState() == OFX_UI_STATE_DOWN){

		setExtractor();

	}

	if (w->getName() == "Get" && w->getState() == OFX_UI_STATE_DOWN){

		getExtractor();
		
	}
	
	if (w->getName() == "actorPreset"){
	
		string sceneName = sceneRadio->getActiveName();
		string presetName = actPresetRadio->getActiveName();
		
		mex.load("presentor/"+sceneName+"_"+presetName+".xml");
	
	}
	
	
	if (w->getName() == "saveActPreset" && w->getState() == OFX_UI_STATE_DOWN){
		
		string sceneName = sceneRadio->getActiveName();
		string presetName = actPresetRadio->getActiveName();
		
		ofDirectory::createDirectory("presentor");
		mex.save("presentor/"+sceneName+"_"+presetName+".xml");
		
	}
}

void ramMEXSync::setExtractor(){

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

void ramMEXSync::getExtractor(){

	mex.clearPorts();
	
	for (int i = 0;i < 2;i++){
		
		if (i == 0) sender.setup(ip_1, 10000);
		if (i == 1) sender.setup(ip_2, 10000);
		
		ofxOscMessage req;
		req.setAddress("/ram/MEX/"+targScene->getTextString()+"/request");
		req.addStringArg(getName());
		
		sender.sendMessage(req);
		
	}
	
}

void ramMEXSync::onEnabled(){
	switcher.selector_hakoniwa->setVisible(true);
	switcher.selector_display->setVisible(true);
}

void ramMEXSync::onDisabled(){
	switcher.selector_hakoniwa->setVisible(false);
	switcher.selector_display->setVisible(false);
}

void ramMEXSync::setActorSort(vector<string> list) {

	mex.setActorList(&list);
	
}

void ramMEXSync::setScene(string scene,bool enable, bool A, bool B){
	((ofxUITextInput*)(switcher.selector_hakoniwa->getWidget("Target")))->setTextString(scene);
	switcher.mEnable = enable;
	switcher.displays[0] = A;
	switcher.displays[2] = B;
	
	switcher.sendMessage();
	
}

void ramMEXSync::setActorPreset(actorPresetMode mode){
	
	if (mode == ACTPRE_STANDARD) actPresetRadio->activateToggle("Standard");
	if (mode == ACTPRE_SOLO) actPresetRadio->activateToggle("Solo");
	if (mode == ACTPRE_DUO) actPresetRadio->activateToggle("Duo");
	if (mode == ACTPRE_TRIO) actPresetRadio->activateToggle("Trio");
	if (mode == ACTPRE_SET_A) actPresetRadio->activateToggle("Preset_A");
	if (mode == ACTPRE_SET_B) actPresetRadio->activateToggle("Preset_B");
	if (mode == ACTPRE_SET_C) actPresetRadio->activateToggle("Preset_C");
	if (mode == ACTPRE_SET_D) actPresetRadio->activateToggle("Preset_D");
	if (mode == ACTPRE_SET_E) actPresetRadio->activateToggle("Preset_E");
	
	string sceneName = sceneRadio->getActiveName();
	string presetName = actPresetRadio->getActiveName();
	
	mex.load("presentor/"+sceneName+"_"+presetName+".xml");
}

#pragma mark ブロックプリセット

void ramMEXSync::blockLaunch(ofxUIEventArgs &e){
	ofxUIWidget* w = e.widget;
	
	if (w->getName().substr(0,7) == "Block_1"){
		vector<string> act;
		act.push_back("kawaguchi");
		act.push_back("kojiri");
		act.push_back("sasamoto");
		setActorSort(act);
		
		setActorPreset(ACTPRE_SOLO);
		setScene("dpHWorm", true, true, true);
		
	}

	if (w->getName().substr(0,7) == "Block_2"){
		setActorPreset(ACTPRE_DUO);
		setScene("dpVisSandStorm", true, false, true);
	}
	
	if (w->getName().substr(0,7) == "Block_3"){
		setActorPreset(ACTPRE_TRIO);
		setScene("dpVisSandStorm", true, true, true);
		setExtractor();
	}
	
	if (w->getName().substr(0,7) == "Block_4"){
		vector<string> act;
		act.push_back("kojiri");
		act.push_back("kawaguchi");
		act.push_back("sasamoto");
		setActorSort(act);
		
		setActorPreset(ACTPRE_DUO);
		setExtractor();
	}
	
	if (w->getName().substr(0,7) == "Block_5"){
		vector<string> act;
		act.push_back("kawaguchi");
		act.push_back("kojiri");
		act.push_back("sasamoto");
		setActorPreset(ACTPRE_STANDARD);
	}
	
	if (w->getName().substr(0,7) == "Block_6"){
		sender.setup("192.168.20.5", 12400);
		ofxOscMessage m;
		m.setAddress("/dp/VisEnable");
		m.addIntArg(0);
		sender.sendMessage(m);
	}
	
	
	if (w->getName().substr(0,7) == "Block_7"){
		vector<string> act;
		act.push_back("kawaguchi");
		act.push_back("kojiri");
		act.push_back("sasamoto");
		setActorSort(act);
		setActorPreset(ACTPRE_DUO);
		setScene("dpVisSandStorm", true, true, true);
		
		ofxOscMessage m;
		m.setAddress("/ram/set_scene");
		m.addStringArg("dpVisSandStorm");
		m.addIntArg(0);
		m.addIntArg(0);
		m.addIntArg(0);
		
		sender.setup(ip_1, 10000);
		sender.sendMessage(m);
		sender.setup(ip_2, 10000);
		sender.sendMessage(m);
	}
	
	if (w->getName().substr(0,7) == "Block_8"){
		vector<string> act;
		act.push_back("kojiri");
		act.push_back("kawaguchi");
		act.push_back("sasamoto");
		setActorSort(act);
		setActorPreset(ACTPRE_DUO);
		setScene("dpVisSandStorm", true, true, true);
		
		ofxOscMessage m;
		m.setAddress("/ram/set_scene");
		m.addStringArg("dpVisSandStorm");
		m.addIntArg(0);
		m.addIntArg(0);
		m.addIntArg(0);
		
		sender.setup(ip_1, 10000);
		sender.sendMessage(m);
		sender.setup(ip_2, 10000);
		sender.sendMessage(m);
	}

	if (w->getName().substr(0,7) == "Block_9"){
		setActorPreset(ACTPRE_TRIO);
		setScene("dpVisSandStorm", true, true, true);
		
		ofxOscMessage m;
		m.setAddress("/ram/set_scene");
		m.addStringArg("dpVisSandStorm");
		m.addIntArg(0);
		m.addIntArg(0);
		m.addIntArg(0);
		
		sender.setup(ip_1, 10000);
		sender.sendMessage(m);
		sender.setup(ip_2, 10000);
		sender.sendMessage(m);
	}
}

void ramMEXSync::drawDump(){
	
	ofPushMatrix();
	ofTranslate(850, 10);
	
	for (int i = 0;i < mex.getNumPort();i++){
		ofPushMatrix();
		ofTranslate(0, i*90);
		
		ofVec3f vec = mex.getVelocityAt(i);
		float speed = mex.getVelocitySpeedAt(i);
		
		ofSetColor(0, 0, 0);
		ofRect(0, 0, 200, 80);
		ofSetColor(255);
		
		ofNoFill();
		ofRect(0, 0, 200, 80);
		ofFill();
		
		string info = "";
		info += "Port  :" + ofToString(i) + "\n";
		info += "Actor :" + mex.getActorNameAt(i) + "\n";
		info += "Joint :" + mex.getJointNameAt(i) + "\n";
		info += "Speed :" + ofToString(mex.getVelocitySpeedAt(i)) + "\n";
		
		ofSetColor(100);
		ofRect(10, 45, mex.getVelocitySpeedAt(i)*10.0, 15);
		
		ofSetColor(255);
		ofDrawBitmapString(info, 10, 15);
		
		
		ofPopMatrix();
	}
	
	ofPopMatrix();
	
}