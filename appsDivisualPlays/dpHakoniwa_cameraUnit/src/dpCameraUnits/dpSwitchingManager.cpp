//
//  dpSwitchingManager.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2015/01/09.
//
//

#include "dpSwitchingManager.h"

void dpSwitchingManager::setup(){
	//箱庭プリセット
	hakoniwaPresets* hako;

	//あばれる君
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_STRUGGLE;
	hakoniwas.back()->CVPreset	= "struggle.xml";
	hakoniwas.back()->sourceCh	= 0;

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_FROZENICE;
	hakoniwas.back()->CVPreset	= "frozen.xml";
	hakoniwas.back()->sourceCh	= 1;

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_LASER;
	hakoniwas.back()->CVPreset	= "frozen.xml";
	hakoniwas.back()->sourceCh	= 2;

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_MAGNET;
	hakoniwas.back()->CVPreset	= "frozen.xml";
	hakoniwas.back()->sourceCh	= 3;

	for (int i = 0;i < 4;i++){
		mSlots[i].targetDisplay.clear();
	}
	mSlots[0].matrixInputCh = CVSW_1;
	mSlots[1].matrixInputCh = CVSW_2;
	mSlots[2].matrixInputCh = CVSW_3;
	mSlots[3].matrixInputCh = CVSW_4;

	isSlave = false;
//	senderToSlave.setup("192.168.20.36", 10000);
}

void dpSwitchingManager::update(){

	if (isSlave){

	}else{

	}
	if (ofGetKeyPressed('5')) matrixSW.setSW(0, 0);
	if (ofGetKeyPressed('6')) matrixSW.setSW(1, 0);
	if (ofGetKeyPressed('7')) matrixSW.setSW(2, 0);
	if (ofGetKeyPressed('8')) matrixSW.setSW(3, 0);
	if (ofGetKeyPressed('9')) matrixSW.setSW(4, 0);
}

void dpSwitchingManager::draw(){
	ofSetColor(0);
	ofPushMatrix();
	ofTranslate(640, 0);
	ofRect(0, 0, 640, 480);

	ofSetColor(255);
	for (int i = 0;i < 4;i++){
		string info = "";
		info += "hakoType :" + ofToString(mSlots[i].hakoType) + "\n";
		info += "sourceCh :" + ofToString(mSlots[i].sourceCh) + "\n";
		info += "Disp :";
		for (int j = 0;j < mSlots[i].targetDisplay.size();j++){
			info += ofToString(mSlots[i].targetDisplay[j]);
		}
		info += "\n";
		info += "isEmpt :" + ofToString(mSlots[i].isEmpty);
		ofDrawBitmapString(info, 20+i*100,20);
	}

	ofPopMatrix();
}

void dpSwitchingManager::receiveOscMessage(ofxOscMessage &m){


	if (m.getAddress() == "/dp/master/switch/enable"){
		SelectHakoniwa(hakoniwaType(m.getArgAsInt32(0)),
					   m.getArgAsInt32(1));

		ofxOscMessage mm;
		mm.setAddress("/dp/toSlave/switch/enable");
		mm.addIntArg(m.getArgAsInt32(0));
		mm.addIntArg(m.getArgAsInt32(1));
		senderToSlave.sendMessage(mm);
	}

	if (m.getAddress() == "/dp/master/switch/disable"){

	}

	if (m.getAddress() == "/dp/toSlave/switch/enable"){
		isSlave = true;
		SelectHakoniwa(hakoniwaType(m.getArgAsInt32(0)),
					   m.getArgAsInt32(1));
	}

}

void dpSwitchingManager::SelectHakoniwa(hakoniwaType type, int slot){

	cout << "SelHako " << type << ":" << slot << endl;
	cout << "Call SelectHakoniwa===" << endl;
	//既に該当スロットがあった場合ディスプレイのみ追加
	int targCvSlot = 0;
	bool isExist = false;
	for (int i = 0;i < 4;i++){
		if (!mSlots[i].isEmpty && mSlots[i].hakoType == type){
			targCvSlot = i;
			isExist = true;
			break;
		}
	}

	if (!isExist){
		cout << "===New Hakoniwa===" << endl;
		enableDisplay(type, slot, !isExist);
		//空いてるCVスロットを見つけて保持
		//CVスロットを初期化
		for (int i = 0;i < 4;i++){
			if (mSlots[i].isEmpty){
				targCvSlot = i;
				break;
			}
		}
		hakoniwaPresets* targHako = getHakoniwaPreset(type);
		mSlots[targCvSlot].isEmpty = false;
		mSlots[targCvSlot].hakoType = type;
		mSlots[targCvSlot].sourceCh = targHako->sourceCh;
		mSlots[targCvSlot].presetFile = targHako->CVPreset;
		mSlots[targCvSlot].targetDisplay.clear();
		mSlots[targCvSlot].targetDisplay.push_back(int(slot));

		matrixSW.setSW(targHako->sourceCh,
					   mSlots[targCvSlot].matrixInputCh);
		cout << "clear & add " << slot << endl;
	}else{

		cout << "===Exist Hakoniwa===" << endl;

		for (int i = 0;i < mSlots[targCvSlot].targetDisplay.size();i++){
			if (mSlots[targCvSlot].targetDisplay[i] == slot) return;
		}

		enableDisplay(type, slot, !isExist);
		mSlots[targCvSlot].targetDisplay.push_back(int(slot));

		cout << "add " << slot << endl;
	}


}

void dpSwitchingManager::enableDisplay(hakoniwaType type, int displayNum,bool newHako){
	//ターゲット位置のディスプレイにスルー信号と箱ビズ信号を送る

	disableDisplay(displayNum);

	ofxOscMessage m;
	m.setAddress("/dp/sceneManage/enable");
	m.addIntArg(type);
	m.addIntArg(displayNum);
	m.addIntArg(newHako);//新規に箱庭アウトシーンを有効にする必要があるかどうか

	//箱庭の映像を舞台ディスプレイへ
	matrixSW.setSW(getHakoniwaPreset(type)->sourceCh, displayNum+4);
	//TODO: RDTKへのOSC送り

}

void dpSwitchingManager::disableHakoniwa(hakoniwaType type){

	int targCvSlot = 0;
	bool isExist = false;
	for (int i = 0;i < 4;i++){
		if ((!mSlots[i].isEmpty) && (mSlots[i].hakoType == type)){
			targCvSlot = 1;
			isExist = true;
			break;
		}
	}

	if (!isExist) return; //該当する箱庭無し

	//ターゲットディスプレイを全て無効にする
	for (int i = 0;i < mSlots[targCvSlot].targetDisplay.size();i++){
		ofxOscMessage m;
		m.setAddress("/dp/sceneManage/disable");
		m.addIntArg(type);
		m.addIntArg(mSlots[targCvSlot].targetDisplay[i]);
	}

	//cvスロットを無効にする
	mSlots[targCvSlot].isEmpty = true;
}

void dpSwitchingManager::disableDisplay(int displayNum){

	bool isExist = false;
	int targCvSlot = -1;
	int targDisp = -1;
	for (int i = 0;i < 4;i++){
		for (int j = 0;j < mSlots[i].targetDisplay.size();j++){
			if (displayNum == mSlots[i].targetDisplay[j]){
				isExist = true;
				targCvSlot = i;
				targDisp = j;
				break;
			}
		}
	}

	if (isExist && targCvSlot > -1 && targDisp > -1){
		int eraseTarg = mSlots[targCvSlot].targetDisplay[targDisp];
		mSlots[targCvSlot].targetDisplay.erase(mSlots[targCvSlot].targetDisplay.begin()+
											   targDisp);
		cout << "Erase :" << eraseTarg << endl;
		ofxOscMessage m;
		m.setAddress("/dp/sceneManage/disable");
		m.addIntArg(mSlots[targCvSlot].hakoType);
		m.addIntArg(displayNum);

		//ディスプレイが無くなった時スロットを無効にする
		if (mSlots[targCvSlot].targetDisplay.size() == 0){
			mSlots[targCvSlot].isEmpty = true;
			mSlots[targCvSlot].hakoType = HAKO_BLANK;
			mSlots[targCvSlot].sourceCh -1;
			mSlots[targCvSlot].presetFile = "";
		}
	}
}

hakoniwaPresets* dpSwitchingManager::getHakoniwaPreset(hakoniwaType type){
	for (int i = 0;i < hakoniwas.size();i++){
		if (hakoniwas[i]->type == type){
			return hakoniwas[i];
		}
	}
}