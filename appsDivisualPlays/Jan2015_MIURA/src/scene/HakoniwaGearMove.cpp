//
//  HakoniwaGearMove.cpp
//  RAMDanceToolkit
//
//  Created by ycam on 2014/12/25.
//
//

#include "HakoniwaGearMove.h"

void HakoniwaGearMove::setupControlPanel(){
    
    mDatahow = true;
    mturn = false;
    mManual = false;
    mgear1 = true;
    mgear2 = true;
    mgear3 = true;
    mturnReverse = true;
    mgear1Reverse = true;
    mgear2Reverse = true;
    mgear3Reverse = true;
    
    Max1speed = 18000;
    Min1speed = 600;
    Max2speed = 30000;
    Min2speed = 100;
    Max3speed = 26000;
    Min3speed = 16000;
    mScale = 3000;
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    panel->addToggle("data show", &mDatahow);
    panel->addToggle("allturn", &mturn);
    panel->addToggle("allturn reverse", &mturnReverse);
    panel->addIntSlider("TrurnSpeed", 100, 35000, &TurnSpeed);
    panel->addToggle("manual turn", &mManual);
    panel->addToggle("gear1", &mgear1);
    panel->addToggle("gear1 reverse", &mgear1Reverse);
    panel->addIntSlider("TrurnGear1Speed", 100, 25000, &TurnGear1Speed);
    panel->addToggle("gear2", &mgear2);
    panel->addToggle("gear2 reverse", &mgear2Reverse);
    panel->addIntSlider("TrurnGear2Speed", 100, 45000, &TurnGear2Speed);
    panel->addToggle("gear3", &mgear3);
    panel->addToggle("gear3 reverse", &mgear3Reverse);
    panel->addIntSlider("TrurnGear3Speed", 100, 35000, &TurnGear3Speed);
    
    ofAddListener(panel->newGUIEvent, this, &HakoniwaGearMove::onPanelChanged);
    motionExtractor.setupControlPanel(this,ofPoint(340,30));
    motionExtractor.load("motionExt_HakoniwaGearMove.xml");
    
}

void HakoniwaGearMove::setup(){
    
    stepManager.setupOsc("192.168.20.51", 8528);
    
    stepManager.addStepper("unit1", 400, 0);
    stepManager.addStepper("unit2", 400, 1);
    stepManager.addStepper("unit3", 400, 2);
    stepManager.resetAllDevices();
//    stepManager.setupEasyFromPreset(KSMR_STEP_SM_42BYG011_25);
    stepManager.setupEasy();
    stepManager.setMicroSteps(5);
    
    stepManager.setStepperAll(true);
    stepManager.absPos(0);
    stepManager.hardStop();

}

void HakoniwaGearMove::update(){

    motionExtractor.update();
    
}

void HakoniwaGearMove::draw(){

    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。

    if (mDatahow)		drawDump();


    if (ofGetFrameNum() % 300 == 0){
        TurnGear1Count = 0;
        TurnGear2Count = 0;
        TurnGear3Count = 0;
        for (int i = 0;i < motionExtractor.getNumPort();i++){
            if (motionExtractor.getActorNameAt(i) == "Default"){
                TurnGear1Count += motionExtractor.getVelocitySpeedAt(i);
            }
            if (motionExtractor.getActorNameAt(i) == "Default1"){
                TurnGear2Count += motionExtractor.getVelocitySpeedAt(i);
            }
            if (motionExtractor.getActorNameAt(i) == "Default2"){
                TurnGear3Count += motionExtractor.getVelocitySpeedAt(i);
            }
        }
        
        if (mManual != true) {
            TurnGear1Speed = TurnGear1Count * mScale;
            TurnGear2Speed = TurnGear2Count * mScale;
            TurnGear3Speed = TurnGear3Count * mScale;
            cout << "TurnGear1Speed " << TurnGear1Speed << endl;
            cout << "TurnGear2Speed " << TurnGear2Speed << endl;
            cout << "TurnGear3Speed " << TurnGear3Speed << endl;
        }
        
        if(mturn == true){
            stepManager.setStepperAll(true);
            stepManager.run(TurnSpeed, mturnReverse);
            stepManager.setStepperAll(false);
        }
        if(mgear1 == true){
            if (mManual != true) {
                float x = ofRandom(0,1);
                int Min1speed = ofRandom(600, 2500);
                if ( x  > 0.51 ){
                    mgear1Reverse = true;
                }else{
                    mgear1Reverse = false;
                }
                if ( TurnGear1Speed >= Max1speed) {
                    OnestepTurn(0, Max1speed, mgear1Reverse);
                }else if( TurnGear1Speed <= Min1speed){
                    OnestepTurn(0, Min1speed, mgear1Reverse);
                }else{
                    OnestepTurn(0, TurnGear1Speed, mgear1Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(0, TurnGear1Speed, mgear1Reverse);
            }
        }
        if(mgear2 == true){
            if (mManual != true) {
                float x = ofRandom(0,1);
                int Min2speed = ofRandom(100, 2500);
                if ( x > 0.51 ){
                    mgear2Reverse = true;
                }else{
                    mgear2Reverse = false;
                }
                if ( TurnGear2Speed >= Max2speed) {
                    OnestepTurn(1, Max2speed, mgear2Reverse);
                }else if( TurnGear2Speed <= Min2speed){
                    OnestepTurn(1, Min2speed, mgear2Reverse);
                }else{
                    OnestepTurn(1, TurnGear2Speed, mgear2Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(1, TurnGear2Speed, mgear2Reverse);
            }
        }
        if(mgear3 == true){
            if (mManual != true) {
                //逆回転はしない
    //            float x = ofRandom(0,1);
    //            if ( x > 0.51 ){
    //                mgear3Reverse = true;
    //            }else{
    //                mgear3Reverse = false;
    //            }
                if ( TurnGear3Speed >= Max3speed) {
                    OnestepTurn(2, Max3speed, true);
                }else if( TurnGear3Speed <= Min3speed){
                    OnestepTurn(2, Min3speed, true);
                }else{
                    OnestepTurn(2, TurnGear3Speed, mgear3Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(2, TurnGear3Speed, mgear3Reverse);
            }
        }
    }
    
}

void HakoniwaGearMove::onPanelChanged(ofxUIEventArgs& e){
    const string name = e.widget->getName();

    if (name == "allturn") {
        if (mturn != true) {
            cout << "mturn off " << endl;
            AllstepTurnStop();
        }
    }else if(name == "gear1"){
        if (mgear1 != true) {
            cout << "gear1 off " << endl;
            OnestepTurnStop(0);
        }
    }else if(name == "gear2"){
        if (mgear2 != true) {
            cout << "gear2 off " << endl;
            OnestepTurnStop(1);
        }
    }else if(name == "gear3"){
        if (mgear3 != true) {
            cout << "gear3 off " << endl;
            OnestepTurnStop(2);
        }
    }
    
    if(name == "allturn reverse"){
        if (mturn == true) {
            AllstepTurn(TurnSpeed, mturnReverse);
        }
    }
    if(name == "gear1 reverse"){
        if (mgear1 == true) {
            OnestepTurn(0, TurnGear1Speed, mgear1Reverse);
        }
    }
    if(name == "gear2 reverse"){
        if (mgear2 == true) {
            OnestepTurn(1, TurnGear2Speed, mgear2Reverse);
        }
    }
    if(name == "gear3 reverse"){
        if (mgear3 == true) {
            OnestepTurn(2, TurnGear3Speed, mgear3Reverse);
        }
    }

}

void HakoniwaGearMove::onDisabled(){
    
    cout << "ondisebled " << endl;
    stepManager.setStepperAll(true);
    stepManager.softStop();

}

void HakoniwaGearMove::AllstepTurn(int speed, bool dir){
    
    stepManager.setStepperAll(true);
    stepManager.run(speed, dir);
    stepManager.setStepperAll(false);

}

void HakoniwaGearMove::OnestepTurn(int ch, int speed, bool dir){
    
    stepManager.selectStepperOne(ch, true);
    stepManager.run(speed, dir);
    stepManager.setStepperAll(false);
    
}

void HakoniwaGearMove::AllstepTurnStop(){
    
    stepManager.setStepperAll(true);
    stepManager.hardStop();
    stepManager.setStepperAll(false);
    
}

void HakoniwaGearMove::OnestepTurnStop(int ch){
    
    stepManager.selectStepperOne(ch, true);
    stepManager.hardStop();
    stepManager.setStepperAll(false);
   
}

void HakoniwaGearMove::drawDump(){
    
    int motionExtCnt = 0;
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        ofPushMatrix();
        if (motionExtCnt >= 12 and motionExtCnt < 16) {
            int j = i -12;
            ofTranslate(1000, j*75);
        }else if(motionExtCnt >= 16 and motionExtCnt < 28){
            int k = i -16;
            ofTranslate(1200, k*75);
        }else if(motionExtCnt >= 28 and motionExtCnt < 32){
            int l = i -28;
            ofTranslate(1400, l*75);
        }else if(motionExtCnt >= 32 and motionExtCnt < 44){
            int m = i -32;
            ofTranslate(1600, m*75);
        }else if(motionExtCnt >= 44){
            int n = i -44;
            ofTranslate(1800, n*75);
        }else{
            ofTranslate(800, i*75);
        }
        motionExtCnt++;
        
        
        ofNoFill();
        ofRect(0, 0, 200, 75);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + motionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + motionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(motionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, motionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        ofPopMatrix();
    }
}