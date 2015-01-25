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

#pragma mark ★パラレルリンク:プリズム
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_PRISM;
	hakoniwas.back()->CVPreset	= "Plink_Prism";
	hakoniwas.back()->sourceCh	= 2;
	hakoniwas.back()->sceneNames.push_back("H:dpHPLink_Prism");
	hakoniwas.back()->sceneNames.push_back("V:dpVisPLink_Prism");

#pragma mark ★パラレルリンク:畜光レーザー
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_LASER;
	hakoniwas.back()->CVPreset	= "Plink_Laser";
	hakoniwas.back()->sourceCh	= 2;
	hakoniwas.back()->sceneNames.push_back("H:dpHPLink_Laser");
	hakoniwas.back()->sceneNames.push_back("V:dpVisPLink_Laser");

#pragma mark ★パラレルリンク:オイル
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_PLINK_OIL;
	hakoniwas.back()->CVPreset	= "Plink_Oil";
	hakoniwas.back()->sourceCh	= 9;
	hakoniwas.back()->sceneNames.push_back("H:dpHPLink_Oil");
	hakoniwas.back()->sceneNames.push_back("V:dpVisPLink_Oil");

#pragma mark サーボ振り子
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_SERVOPENDULUM;
	hakoniwas.back()->CVPreset	= "ServoPendulum";
	hakoniwas.back()->sourceCh	= 6;
	hakoniwas.back()->sceneNames.push_back("H:dpHServoPendulum");
	hakoniwas.back()->sceneNames.push_back("V:dpVisServoPendulum");

#pragma mark 磁石振り子
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_MAGPENDULUM;
	hakoniwas.back()->CVPreset	= "MagPendulum";
	hakoniwas.back()->sourceCh	= 4;
	hakoniwas.back()->sceneNames.push_back("H:dpHMagPendulum");
    hakoniwas.back()->sceneNames.push_back("H:dpHEyeBall");
	hakoniwas.back()->sceneNames.push_back("V:dpVisMagPendulum");

#pragma mark Theta
	//TODO: シーンV名
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_THETA;
	hakoniwas.back()->CVPreset	= "Theta";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:dpVisTheta");

#pragma mark ★色水
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_COLOROFWATER;
	hakoniwas.back()->CVPreset	= "ColorOfWater";
	hakoniwas.back()->sourceCh	= 1;
	hakoniwas.back()->sceneNames.push_back("H:dpHColorOfWater");

#pragma mark 砂嵐
	//TODO: ソース番号
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_SANDSTORM;
	hakoniwas.back()->CVPreset	= "SandStorm";
	hakoniwas.back()->sourceCh	= 3;
	hakoniwas.back()->sceneNames.push_back("H:dpHSandStorm");
	hakoniwas.back()->sceneNames.push_back("V:dpVisSandStorm");

#pragma mark 氷
	//TODO: ソース番号・箱庭出力シーン
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_FROZENICE;
	hakoniwas.back()->CVPreset	= "Ice";
	hakoniwas.back()->sourceCh	= 9;
	hakoniwas.back()->sceneNames.push_back("H:dpHfrozenIce");
	hakoniwas.back()->sceneNames.push_back("V:dpVisIce");

#pragma mark 尺取り虫
	//TODO: ソース番号・箱庭出力シーン
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_WORM;
	hakoniwas.back()->CVPreset	= "Worm";
	hakoniwas.back()->sourceCh	= 5;
	hakoniwas.back()->sceneNames.push_back("H:dpHWorm");
	hakoniwas.back()->sceneNames.push_back("V:dpVisWorm");

#pragma mark ★ステージ
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_STAGE;
	hakoniwas.back()->CVPreset	= "Stage";
	hakoniwas.back()->sourceCh	= 10;
	hakoniwas.back()->sceneNames.push_back("V:dpVisStage");

#pragma mark 暴れる
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type      = HAKO_STRUGGLE;
	hakoniwas.back()->CVPreset  = "Struggle";
	hakoniwas.back()->sourceCh  = 7;
	hakoniwas.back()->sceneNames.push_back("H:dpHStruggle");
	hakoniwas.back()->sceneNames.push_back("V:dpVisStruggle");

#pragma mark ギア
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_GEAR;
	hakoniwas.back()->CVPreset	= "Gear";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("H:dpHGearMove");
	hakoniwas.back()->sceneNames.push_back("V:dpVisGearMove");

#pragma mark トルネード
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_TORNADO;
	hakoniwas.back()->CVPreset	= "Tornado";
	hakoniwas.back()->sourceCh	= 8;
	hakoniwas.back()->sceneNames.push_back("H:dpHTornado");
	hakoniwas.back()->sceneNames.push_back("V:dpVisTornado");

#pragma mark メタボール
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_METABALL;
	hakoniwas.back()->CVPreset	= "Metaball";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:distanceMetaball");

#pragma mark ライン
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_LINE;
	hakoniwas.back()->CVPreset	= "Line";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:Line");

#pragma mark フォーポイント
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_FOURPOINT;
	hakoniwas.back()->CVPreset	= "FourPoint";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:Four Points");

#pragma mark 未来
	hakoniwas.push_back(new hakoniwaPresets());
    hakoniwas.back()->type      = HAKO_FUTURE;
	hakoniwas.back()->CVPreset	= "FutureRE";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:FutureRE");
    
#pragma mark Future legacy
    hakoniwas.push_back(new hakoniwaPresets());
    hakoniwas.back()->type      = HAKO_FUTURE_LEG;
    hakoniwas.back()->CVPreset  = "Future";
    hakoniwas.back()->sourceCh = SHUTTER_CHANNEL;
    hakoniwas.back()->sceneNames.push_back("V:Future");

#pragma mark onNote
    hakoniwas.push_back(new hakoniwaPresets());
    hakoniwas.back()->type      = HAKO_ONNOTE;
    hakoniwas.back()->CVPreset  = "OnNote";
	hakoniwas.back()->sourceCh  = 10;
    hakoniwas.back()->sceneNames.push_back("H:OnNote");

#pragma mark あいボールダンサー
    hakoniwas.push_back(new hakoniwaPresets());
    hakoniwas.back()->type      = HAKO_EYEBALLDANCER;
    hakoniwas.back()->CVPreset  = "EyeBallDancer";
    hakoniwas.back()->sourceCh  = SHUTTER_CHANNEL;
    hakoniwas.back()->sceneNames.push_back("H:dpHEyeBallDancer");
    
#pragma mark 記憶
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_KIOKU;
	hakoniwas.back()->CVPreset	= "Kioku";
	hakoniwas.back()->sourceCh	= SHUTTER_CHANNEL;
	hakoniwas.back()->sceneNames.push_back("V:Kioku");

#pragma mark ★テストA
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_TESTA;
	hakoniwas.back()->CVPreset	= "TestA";
	hakoniwas.back()->sourceCh	= 2;
	hakoniwas.back()->sceneNames.push_back("V:TestSceneA");

#pragma mark ★テストB
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_TESTB;
	hakoniwas.back()->CVPreset	= "TestB";
	hakoniwas.back()->sourceCh	= 3;
	hakoniwas.back()->sceneNames.push_back("V:TestSceneB");

#pragma mark ★テストC
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_TESTC;
	hakoniwas.back()->CVPreset	= "TestC";
	hakoniwas.back()->sourceCh	= 4;
	hakoniwas.back()->sceneNames.push_back("V:TestSceneC");

#pragma mark ★テストD
	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_TESTD;
	hakoniwas.back()->CVPreset	= "TestD";
	hakoniwas.back()->sourceCh	= 10;
	hakoniwas.back()->sceneNames.push_back("V:TestSceneD");

	hakoniwas.push_back(new hakoniwaPresets());
	hakoniwas.back()->type		= HAKO_BLANK;
	hakoniwas.back()->CVPreset	= "blank";
	hakoniwas.back()->sourceCh	= -1;
	hakoniwas.back()->sceneNames.push_back("Blank");

	for (int i = 0;i < CV_SLOT_NUM;i++){
		mSlots[i].targetDisplay.clear();
	}
	mSlots[0].matrixInputCh = CVSW_1;
	mSlots[1].matrixInputCh = CVSW_2;
	mSlots[2].matrixInputCh = CVSW_3;
	mSlots[3].matrixInputCh = CVSW_4;

	isSlave = false;
	if (!NETWORK_ISSTOP){
		senderToSlave.setup("192.168.20.4", 12400);
		//    senderToSlave.setup("Hampshire.local", 12400);

		senderToRDTK1.setup("192.168.20.3", 10000);
		senderToRDTK2.setup("192.168.20.2", 10000);
        senderToFloor.setup("192.168.20.6", 10000);
	}

	mVisEnable = true;
}

void dpSwitchingManager::update(){
//	if (ofGetKeyPressed('a')){
//		ofxOscMessage m;
//		m.setAddress("/ram/set_scene");
//		m.addStringArg("dpHPLink_Laser");
//		m.addIntArg(1);
//		m.addIntArg(1);
//		m.addIntArg(1);
//		receiveOscMessage(m);
//	}
	if (ofGetKeyPressed('c')){

	}

	if (ofGetFrameNum() % 5 == 0 && oscListPtr != NULL && totalManage){

		ofxOscMessage Live;
		Live.setAddress("/dp/caemraUnit/aliveMonitor");
		multiCast(Live);

        cout << "SceneState=====" ;
		for (int j = 0;j < hakoniwas.size();j++){
			ofxOscMessage current;
			current.setAddress("/dp/cameraUnit/sceneState/"+hakoniwas[j]->CVPreset);
            if (hakoniwas[j]->isEnable){
                current.addIntArg(1);
            }else{
                current.addIntArg(0);
            }
			multiCast(current);
            cout << (hakoniwas[j]->isEnable ? 1 : 0);
		}
        cout << endl;
	}

    
    for (int i = 0;i < 4;i++){
        string dispInfo = "";
        
        for (int j = 0;j < CV_SLOT_NUM;j++){
            for (int o = 0;o < mSlots[j].targetDisplay.size();o++){
                if (mSlots[j].targetDisplay[o] == i){
                    dispInfo += getHakoniwaPreset(mSlots[j].hakoType)->sceneNames[0].substr(2);
                }
            }
        }
        
        
        ofxOscMessage m;
        m.setAddress("/dp/cameraUnit/display");
        m.addIntArg(i);
        m.addStringArg(dispInfo);
        
        multiCast(m);
    }
}

void dpSwitchingManager::draw(){
	ofSetColor(0);
	ofPushMatrix();
	ofTranslate(640, 680);

	ofRect(0, 0, 640, 480);


	ofSetColor(255);
	for (int i = 0;i < CV_SLOT_NUM;i++){
		ofPushMatrix();
		ofTranslate(i*120, 0);
		string info = "";
		info += "hakoType :" + ofToString(mSlots[i].hakoType) + "\n";
		info += "sourceCh :" + ofToString(mSlots[i].sourceCh) + "\n";
		info += "Disp :";
		for (int j = 0;j < mSlots[i].targetDisplay.size();j++){
			info += ofToString(mSlots[i].targetDisplay[j]);
		}
		info += "\n";
		info += "isEmpt :" + ofToString(mSlots[i].isEmpty);
		ofSetColor(128+!mSlots[i].isEmpty * 128);
		ofDrawBitmapString(info, 10,20);

		ofNoFill();
		ofRect(0, 0, 120, 100);
		ofFill();
		ofPopMatrix();
	}

	for (int i = 0;i < 4;i++){
		string dispInfo = "";

		for (int j = 0;j < CV_SLOT_NUM;j++){
			for (int o = 0;o < mSlots[j].targetDisplay.size();o++){
				if (mSlots[j].targetDisplay[o] == i){
					dispInfo += getHakoniwaPreset(mSlots[j].hakoType)->sceneNames[0].substr(2);
				}
			}
		}

		ofPushMatrix();
		ofTranslate(i%2*120, 150+i/2*90);
		ofSetColor(255);
		ofDrawBitmapString(dispInfo, 0,0);

		if (dispInfo == "TestSceneA") ofSetColor(255, 0, 0);
		if (dispInfo == "TestSceneC") ofSetColor(0, 255, 0);
		if (dispInfo == "TestSceneB") ofSetColor(255, 255, 0);
		if (dispInfo == "TestSceneD") ofSetColor(255, 0, 255);
		ofRect(0, 20, 30, 30);

		ofPopMatrix();
        
        ofxOscMessage m;
        m.setAddress("/dp/cameraUnit/display");
        m.addIntArg(i);
        m.addStringArg(dispInfo);
        
        multiCast(m);
	}

	ofPopMatrix();
}

void dpSwitchingManager::receiveOscMessage(ofxOscMessage &m){
    string ad = m.getAddress();
    if (m.getAddress() == "/dp/gearMove/fingerpress/" ||
        ad.substr(0,16) == "/dp/magnetLooper"){
        cout << "Through out" << m.getAddress() << endl;
        sender.setup("192.168.20.10", 10000);
        sender.sendMessage(m);
    }
    
	if (m.getAddress() == "/ram/set_slave"){
		isSlave = true;
		m.setAddress("/ram/set_scene");
	}

	if (m.getAddress() == "/dp/VisEnable"){
		mVisEnable = (m.getArgAsInt32(0) == 1);
		
		if (!mVisEnable){
			refleshSceneforRDTK();
			matrixSW.setSW(SHUTTER_CHANNEL, 0+5);
			matrixSW.setSW(SHUTTER_CHANNEL, 1+5);
			matrixSW.setSW(SHUTTER_CHANNEL, 2+5);
			matrixSW.setSW(SHUTTER_CHANNEL, 3+5);
		}
	}
	
	if (m.getAddress() == "/ram/set_scene"){
		cout << "=-=-=-=-=-=-=-Head -=-=-=-=-=-=-=-=" << endl << endl;
		int hakoId = getHakoniwaIndex(m.getArgAsString(0));
		cout << "hakoID :" << hakoId << endl;
		if (m.getArgAsInt32(1)){
			//箱庭を探して有効化
			if (hakoId > -1){
				for (int i = 0;i < CV_SLOT_NUM;i++){
                    if (m.getArgAsInt32(2+(i < 2 ? 0 : 1)) != 0){
						cout << "Select hakoniwa from Mastre=====" << endl;
						SelectHakoniwa(hakoniwaType(hakoId), i);
					}else{
						cout << "Disable Disp from Master=====" << endl;
						//ターゲット箱庭が有効になっている時だけdisableDisplayを呼ぶ
						int activeSlot = searchHakoniwaIsActive(hakoniwaType(hakoId));
						if (activeSlot > -1){
							for (int q = 0;q < mSlots[activeSlot].targetDisplay.size();q++){
								if (mSlots[activeSlot].targetDisplay[q] == i){
									disableDisplay(i);
								}
							}
						}
					}
				}
			}
		}else{
			//箱庭を探して無効化
			cout << "Disable hakoniwa from Master=====" << endl;
			disableHakoniwa(hakoniwaType(hakoId));
		}
		if (!isSlave){
			m.setAddress("/ram/set_slave");
			senderToSlave.sendMessage(m);
		}
	}

	if (m.getAddress() == "/dp/master/switch/enable"){
		SelectHakoniwa(hakoniwaType(m.getArgAsInt32(0)),
					   m.getArgAsInt32(1));

		ofxOscMessage mm;
		mm.setAddress("/dp/toSlave/switch/enable");
		mm.addIntArg(m.getArgAsInt32(0));
		mm.addIntArg(m.getArgAsInt32(1));
		if (!NETWORK_ISSTOP) senderToSlave.sendMessage(mm);
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
	for (int i = 0;i < CV_SLOT_NUM;i++){
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
		for (int i = 0;i < CV_SLOT_NUM;i++){
			if (mSlots[i].isEmpty){
				targCvSlot = i;
				break;
			}
		}
		hakoniwaPresets* targHako = getHakoniwaPreset(type);
		targHako->isEnable = true;
		mSlots[targCvSlot].isEmpty = false;
		mSlots[targCvSlot].hakoType = type;
		mSlots[targCvSlot].sourceCh = targHako->sourceCh;
		mSlots[targCvSlot].presetFile = targHako->CVPreset;
		mSlots[targCvSlot].targetDisplay.clear();
		mSlots[targCvSlot].targetDisplay.push_back(int(slot));

		if (!isSlave){
			if (targCvSlot == 0 || targCvSlot == 1 || targCvSlot == 2){
				FXPtr[targCvSlot]		.loadPreset(mSlots[targCvSlot].presetFile);
				AnalysisPtr[targCvSlot]	.loadPreset(mSlots[targCvSlot].presetFile);
				for (int i = 0;i < 4;i++)
					AnalysisPtr[i].oscMatrixUI->setToggle(0, 2, false);

				AnalysisPtr[targCvSlot] .oscMatrixUI->setToggle(0, 2, true);
			}
		}else{
			if (targCvSlot == 2 || targCvSlot == 3){
				FXPtr[targCvSlot]		.loadPreset(mSlots[targCvSlot].presetFile);
				AnalysisPtr[targCvSlot]	.loadPreset(mSlots[targCvSlot].presetFile);
				for (int i = 0;i < 4;i++)
					AnalysisPtr[i].oscMatrixUI->setToggle(0, 2, false);

				AnalysisPtr[targCvSlot] .oscMatrixUI->setToggle(0, 2, true);
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

		cout << "add " << slot << endl;
	}

	refleshSceneforRDTK();

}

void dpSwitchingManager::enableDisplay(hakoniwaType type, int displayNum,bool newHako){
	//ターゲット位置のディスプレイにスルー信号と箱ビズ信号を送る

	disableDisplay(displayNum);

	hakoniwaPresets* tp = getHakoniwaPreset(type);

	//箱庭の映像を舞台ディスプレイへ
    int targDSP;
    if (displayNum == 0) targDSP = 1;
    if (displayNum == 1) targDSP = 2;
    
    if (displayNum == 2) targDSP = 0;
    if (displayNum == 3) targDSP = 3;

    if (mVisEnable){
        matrixSW.setSW(getHakoniwaPreset(type)->sourceCh, targDSP+5);
    }else{
        matrixSW.setSW(SHUTTER_CHANNEL, targDSP+5);
    }
	//TODO: RDTKへのOSC送り

	refleshSceneforRDTK();
}

void dpSwitchingManager::disableHakoniwa(hakoniwaType type){

	int targCvSlot = 0;
	bool isExist = false;
	for (int i = 0;i < CV_SLOT_NUM;i++){
		if ((!mSlots[i].isEmpty) && (mSlots[i].hakoType == type)){
			targCvSlot = i;
			isExist = true;
			break;
		}
	}

	if (!isExist){
		refleshSceneforRDTK();
		return; //該当する箱庭無し
	}

	//ターゲットディスプレイを全て無効にする
	for (int i = 0;i < mSlots[targCvSlot].targetDisplay.size();i++){
		disableDisplay(mSlots[targCvSlot].targetDisplay[i]);
	}

	//cvスロットを無効にする
	getHakoniwaPreset(type)->isEnable = false;
	mSlots[targCvSlot].isEmpty	= true;
	mSlots[targCvSlot].hakoType	= HAKO_BLANK;
	mSlots[targCvSlot].targetDisplay.clear();
	mSlots[targCvSlot].sourceCh = - 1;
	mSlots[targCvSlot].presetFile = "";

	refleshSceneforRDTK();
}

void dpSwitchingManager::disableDisplay(int displayNum){

	bool isExist = false;
	int targCvSlot = -1;
	int targDisp = -1;
	for (int i = 0;i < CV_SLOT_NUM;i++){
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

		//ターゲットディスプレイプロパティを消去する
		int eraseTarg = mSlots[targCvSlot].targetDisplay[targDisp];
		mSlots[targCvSlot].targetDisplay.erase(mSlots[targCvSlot].targetDisplay.begin()+
											   targDisp);

		//ディスプレイが無くなった時スロットを無効にする
		if (mSlots[targCvSlot].targetDisplay.size() == 0){
			getHakoniwaPreset(mSlots[targCvSlot].hakoType)->isEnable = false;
			mSlots[targCvSlot].isEmpty	= true;
			mSlots[targCvSlot].hakoType	= HAKO_BLANK;
			mSlots[targCvSlot].sourceCh = - 1;
			mSlots[targCvSlot].presetFile = "";
		}
	}

	refleshSceneforRDTK();
}

hakoniwaPresets* dpSwitchingManager::getHakoniwaPreset(hakoniwaType type){
	for (int i = 0;i < hakoniwas.size();i++){
		if (hakoniwas[i]->type == type){
			return hakoniwas[i];
		}
	}
	return NULL;
}

int dpSwitchingManager::getHakoniwaIndex(string sceneName){
	for (int i = 0;i < hakoniwas.size();i++){
		for (int j = 0;j < hakoniwas[i]->sceneNames.size();j++){
			if (hakoniwas[i]->sceneNames[j].substr(2) == sceneName) return i;
		}
	}

	return -1;
}

void dpSwitchingManager::refleshSceneforRDTK(){

	for (int i = 0;i < hakoniwas.size();i++){
		if (searchHakoniwaIsActive(hakoniwas[i]->type) == -1){
			for (int j = 0;j < hakoniwas[i]->sceneNames.size();j++){
				ofxOscMessage m;
				m.setAddress("/ram/set_scene");
				m.addStringArg(hakoniwas[i]->sceneNames[j].substr(2));
				m.addIntArg(0);
				m.addIntArg(0);
				m.addIntArg(0);
				cout << "Clear :" << hakoniwas[i]->sceneNames[j].substr(2) << endl;
				if (!NETWORK_ISSTOP) senderToRDTK1.sendMessage(m);
				if (!NETWORK_ISSTOP) senderToRDTK2.sendMessage(m);
			}
		}
	}

	for (int i = 0;i < CV_SLOT_NUM;i++){

		if (!mSlots[i].isEmpty){
			hakoniwaPresets* hk = getHakoniwaPreset(mSlots[i].hakoType);

			for (int j = 0;j < hk->sceneNames.size();j++){
				ofxOscMessage m1,m2;
				m1.setAddress("/ram/set_scene");
				m1.addStringArg(hk->sceneNames[j].substr(2));


				if (!hk->getIsVis(j)){
					//箱庭アウト
					m1.addIntArg((hk->type % 2 == 0) &&
								 (searchHakoniwaIsActive(hk->type) > -1));
					m1.addIntArg(1);
					m1.addIntArg(0);
				}else{
					//ビズ
					m1.addIntArg(mSlots[i].displayIsExist(0) ||
								 mSlots[i].displayIsExist(1));
					m1.addIntArg(mSlots[i].displayIsExist(0) && mVisEnable);
					m1.addIntArg(mSlots[i].displayIsExist(1) && mVisEnable);
				}

				m2.setAddress("/ram/set_scene");
				m2.addStringArg(hk->sceneNames[j].substr(2));
				if (!hk->getIsVis(j)){
					m2.addIntArg((hk->type % 2 == 1) &&
								 (searchHakoniwaIsActive(hk->type) > -1));
					m2.addIntArg(0);
					m2.addIntArg(0);
				}else{
					m2.addIntArg(mSlots[i].displayIsExist(3) ||
								 mSlots[i].displayIsExist(2));
					m2.addIntArg(mSlots[i].displayIsExist(3) && mVisEnable);
					m2.addIntArg(mSlots[i].displayIsExist(2) && mVisEnable);
				}
                
                if (hakoniwas[8]->isEnable){
                    ofxOscMessage mf;
                    mf.setAddress("/ram/set_scene");
                    mf.addStringArg("dpVisIce");
                    mf.addIntArg(1);
                    mf.addIntArg(1);
                    mf.addIntArg(1);
                    senderToFloor.sendMessage(mf);
                }
                

				if (!NETWORK_ISSTOP) senderToRDTK1.sendMessage(m1);
				if (!NETWORK_ISSTOP) senderToRDTK2.sendMessage(m2);
			}
		}

	}

}

int dpSwitchingManager::searchHakoniwaIsActive(hakoniwaType type){

	for (int i = 0;i < CV_SLOT_NUM;i++){
		if ((!mSlots[i].isEmpty) &&
			(mSlots[i].hakoType == type)) return i;
	}
	return -1;

}

void dpSwitchingManager::multiCast(ofxOscMessage &m){

	for (int i = 0;i < oscListPtr->size();i++){
		sender.setup((*oscListPtr)[i], 10000);
		sender.sendMessage(m);
	}

}