//
//  dpSwitchingManager.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2015/01/09.
//
//

#include "dpSwitchingManager.h"

void dpSwitchingManager::setup(dpCameraUnit_cvFX* fxP,
							   dpCameraUnit_cvAnalysis* anP){

	FXPtr = fxP;
	AnalysisPtr = anP;
	
	//箱庭プリセット
	hakoniwaPresets* hako;

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_PRISM;
	hakoniwas.back()->CVPreset	= "plink_Prism";
	hakoniwas.back()->sourceCh	= 2;
	hakoniwas.back()->sceneNames.push_back("H:HakoniwaPLink_Prism");
	hakoniwas.back()->sceneNames.push_back("V:dpVisPLinkPrism");

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_LASER;
	hakoniwas.back()->CVPreset	= "plink_Laser";
	hakoniwas.back()->sourceCh	= 3;
	hakoniwas.back()->sceneNames.push_back("H:HakoniwaPLink_Laser");
	hakoniwas.back()->sceneNames.push_back("V:hakoVisPLink_Laser");

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_OIL;
	hakoniwas.back()->CVPreset	= "plink_Oil";
	hakoniwas.back()->sourceCh	= 4;
	hakoniwas.back()->sceneNames.push_back("H:HakoniwaPLink_Oil");
	hakoniwas.back()->sceneNames.push_back("V:hakoVisPLink_Oil");

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_SERVOPENDULUM;
	hakoniwas.back()->CVPreset	= "servoPendulum";
	hakoniwas.back()->sourceCh	= 8;
    hakoniwas.back()->sceneNames.push_back("H:dpHServoPendulum");
    hakoniwas.back()->sceneNames.push_back("V:dpVisServoPendulum");
    

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_STAGE;
	hakoniwas.back()->CVPreset	= "stage";
	hakoniwas.back()->sourceCh	= 10;
	hakoniwas.back()->sceneNames.push_back("V:dpVisStage");

//	hakoniwas.push_back(new hakoniwaPresets());
//	hakoniwas.back()->type		= HAKO_FROZENICE;
//	hakoniwas.back()->CVPreset	= "frozenIce";
//	hakoniwas.back()->

	for (int i = 0;i < 4;i++){
		mSlots[i].targetDisplay.clear();
	}
	mSlots[0].matrixInputCh = CVSW_1;
	mSlots[1].matrixInputCh = CVSW_2;
	mSlots[2].matrixInputCh = CVSW_3;
	mSlots[3].matrixInputCh = CVSW_4;

	isSlave = false;
    senderToSlave.setup("192.168.20.4", 12400);
//    senderToSlave.setup("Hampshire.local", 12400);

	senderToRDTK1.setup("192.168.20.2", 10000);
	senderToRDTK2.setup("192.168.20.3", 10000);

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
	bool TypeisExist = false;
	for (int i = 0;i < hakoniwas.size();i++){
		//プリセットが未アサインの時はリターン)
		if (hakoniwas[i]->type == type) TypeisExist = true;
	}
	if (!TypeisExist) return;

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

		for (int i = 0;i < targHako->sceneNames.size();i++){
			ofxOscMessage m;
			m.setAddress("/ram/set_scene");
			m.addStringArg(targHako->sceneNames[i].substr(2));
			m.addIntArg(1);
			if (!isSlave){
				int tg = 0;
				//Search A Display
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 0) tg = 1;
				}
				m.addIntArg(tg);

				//Search B Display
				tg = 0;
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 1) tg = 1;
				}
				m.addIntArg(tg);
				senderToRDTK1.sendMessage(m);
			}else{
				int tg = 0;
				//Search A Display
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 2) tg = 1;
				}
				m.addIntArg(tg);

				//Search B Display
				tg = 0;
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 3) tg = 1;
				}
				m.addIntArg(tg);
				senderToRDTK2.sendMessage(m);
			}
		}

        if (!isSlave){
            if (targCvSlot == 0 || targCvSlot == 1){
                FXPtr[targCvSlot]		.loadPreset(mSlots[targCvSlot].presetFile);
                AnalysisPtr[targCvSlot]	.loadPreset(mSlots[targCvSlot].presetFile);
            }
        }else{
            if (targCvSlot == 2 || targCvSlot == 3){
                FXPtr[targCvSlot]		.loadPreset(mSlots[targCvSlot].presetFile);
                AnalysisPtr[targCvSlot]	.loadPreset(mSlots[targCvSlot].presetFile);
            }
        }
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
		hakoniwaPresets* targHako = getHakoniwaPreset(type);

		for (int i = 0;i < targHako->sceneNames.size();i++){
			if (targHako->sceneNames[i].substr(0,1) == "H")	continue;
			ofxOscMessage m;
			m.setAddress("/ram/set_scene");
			m.addStringArg(targHako->sceneNames[i].substr(2));
			m.addIntArg(1);
			if (!isSlave){
				int tg = 0;
				//Search A Display
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 0) tg = 1;
				}
				m.addIntArg(tg);

				//Search B Display
				tg = 0;
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 1) tg = 1;
				}
				m.addIntArg(tg);
				senderToRDTK1.sendMessage(m);
			}else{
				int tg = 0;
				//Search A Display
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 2) tg = 1;
				}
				m.addIntArg(tg);

				//Search B Display
				tg = 0;
				for (int j = 0;j < mSlots[targCvSlot].targetDisplay.size();j++){
					if (mSlots[targCvSlot].targetDisplay[j] == 3) tg = 1;
				}
				m.addIntArg(tg);
				senderToRDTK2.sendMessage(m);
			}
		}

		
		cout << "add " << slot << endl;
	}


}

void dpSwitchingManager::enableDisplay(hakoniwaType type, int displayNum,bool newHako){
	//ターゲット位置のディスプレイにスルー信号と箱ビズ信号を送る

	disableDisplay(displayNum);

	hakoniwaPresets* tp = getHakoniwaPreset(type);

//	if (displayNum == 0 ||
//		displayNum == 1){
//		for (int i = 0;i < tp->sceneNames.size();i++){
//			ofxOscMessage m;
//			m.setAddress("/ram/set_scene");
//			m.addStringArg(tp->sceneNames[i].substr(2));
//
//			if (tp->sceneNames[i].substr(0,1) == "V"){
//				m.addIntArg(1);
//				m.addIntArg(displayNum == 0);//screen0
//				m.addIntArg(displayNum == 1);//screen1
//			}else{
//				m.addIntArg(newHako);
//				m.addIntArg(0);
//				m.addIntArg(0);
//			}
//
//			senderToRDTK1.sendMessage(m);
//		}
//	}
//	if (displayNum == 2 ||
//		displayNum == 3){
//		for (int i = 0;i < tp->sceneNames.size();i++){
//			ofxOscMessage m;
//			m.setAddress("/ram/set_scene");
//			m.addStringArg(tp->sceneNames[i].substr(2));
//
//			if (tp->sceneNames[i].substr(0,1) == "V"){
//				m.addIntArg(1);
//				m.addIntArg(displayNum == 2);//screen0
//				m.addIntArg(displayNum == 3);//screen1
//			}else{
//				m.addIntArg(newHako);
//				m.addIntArg(0);
//				m.addIntArg(0);
//			}
//
//			senderToRDTK1.sendMessage(m);
//		}
//	}


	//箱庭の映像を舞台ディスプレイへ
//	matrixSW.setSW(getHakoniwaPreset(type)->sourceCh, displayNum+4);
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

		//ディスプレイが無くなった時スロットを無効にする
		if (mSlots[targCvSlot].targetDisplay.size() == 0){
			mSlots[targCvSlot].isEmpty	= true;
			mSlots[targCvSlot].hakoType	= HAKO_BLANK;
			mSlots[targCvSlot].sourceCh = - 1;
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
	return NULL;
}