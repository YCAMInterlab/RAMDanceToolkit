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
    mAllTurn = false;
    mManual = false;
    mGear1 = true;
    mGear2 = true;
    mGear3 = true;
    mAllGearReverse = true;
    mGear1Reverse = true;
    mGear2Reverse = true;
    mGear3Reverse = true;
    
    mMax1speed = 18000;
    mMin1speed = 600;
    mMax2speed = 30000;
    mMin2speed = 100;
    mMax3speed = 26000;
    mMin3speed = 16000;
    mScale = 3000;
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    panel->addToggle("data show", &mDatahow);
    panel->addToggle("all gear turn", &mAllTurn);
    panel->addToggle("all gear reverse", &mAllGearReverse);
    panel->addIntSlider("all gear Speed", 100, 35000, &mAllGearSpeed);
    panel->addToggle("manual turn", &mManual);
    panel->addToggle("gear1", &mGear1);
    panel->addToggle("gear1 reverse", &mGear1Reverse);
    panel->addIntSlider("TrurnGear1Speed", 100, 25000, &mGear1Speed);
    panel->addToggle("gear2", &mGear2);
    panel->addToggle("gear2 reverse", &mGear2Reverse);
    panel->addIntSlider("TrurnGear2Speed", 100, 45000, &mGear2Speed);
    panel->addToggle("gear3", &mGear3);
    panel->addToggle("gear3 reverse", &mGear3Reverse);
    panel->addIntSlider("TrurnGear3Speed", 100, 35000, &mGear3Speed);
    
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
//    stepManager.setupEasyFromPreset(KSMR_STEP_P_PMSA_B56D5);
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
        mGear1Count = 0;
        mGear2Count = 0;
        mGear3Count = 0;
        for (int i = 0;i < motionExtractor.getNumPort();i++){
            if (motionExtractor.getActorNameAt(i) == "sasamoto"){
                mGear1Count += motionExtractor.getVelocitySpeedAt(i);
            }
            if (motionExtractor.getActorNameAt(i) == "kojiri"){
                mGear2Count += motionExtractor.getVelocitySpeedAt(i);
            }
            if (motionExtractor.getActorNameAt(i) == "kawaguchi"){
                mGear3Count += motionExtractor.getVelocitySpeedAt(i);
            }
        }
        
        if (mManual != true) {
            mGear1Speed = mGear1Count * mScale;
            mGear2Speed = mGear2Count * mScale;
            mGear3Speed = mGear3Count * mScale;
            cout << "Gear1Speed " << mGear1Speed << endl;
            cout << "Gear2Speed " << mGear2Speed << endl;
            cout << "Gear3Speed " << mGear3Speed << endl;
        }
        
        if(mAllTurn == true){
            stepManager.setStepperAll(true);
            stepManager.run(mAllGearSpeed, mAllGearReverse);
            stepManager.setStepperAll(false);
        }
        if(mGear1 == true){
            if (mManual != true) {
                float x = ofRandom(0,1);
                int Min1speed = ofRandom(600, 2500);
                if ( x  > 0.51 ){
                    mGear1Reverse = true;
                }else{
                    mGear1Reverse = false;
                }
                if ( mGear1Speed >= mMax1speed) {
                    OnestepTurn(0, mMax1speed, mGear1Reverse);
                }else if( mGear1Speed <= mMin1speed){
                    OnestepTurn(0, mMin1speed, mGear1Reverse);
                }else{
                    OnestepTurn(0, mGear1Speed, mGear1Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(0, mGear1Speed, mGear1Reverse);
            }
        }
        if(mGear2 == true){
            if (mManual != true) {
                float x = ofRandom(0,1);
                int Min2speed = ofRandom(100, 2500);
                if ( x > 0.51 ){
                    mGear2Reverse = true;
                }else{
                    mGear2Reverse = false;
                }
                if ( mGear2Speed >= mMax2speed) {
                    OnestepTurn(1, mMax2speed, mGear2Reverse);
                }else if( mGear2Speed <= mMin2speed){
                    OnestepTurn(1, mMin2speed, mGear2Reverse);
                }else{
                    OnestepTurn(1, mGear2Speed, mGear2Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(1, mGear2Speed, mGear2Reverse);
            }
        }
        if(mGear3 == true){
            if (mManual != true) {
                //逆回転はしない
    //            float x = ofRandom(0,1);
    //            if ( x > 0.51 ){
    //                mgear3Reverse = true;
    //            }else{
    //                mgear3Reverse = false;
    //            }
                if ( mGear3Speed >= mMax3speed) {
                    OnestepTurn(2, mMax3speed, true);
                }else if( mGear3Speed <= mMin3speed){
                    OnestepTurn(2, mMin3speed, true);
                }else{
                    OnestepTurn(2, mGear3Speed, mGear3Reverse);
                }
            }else{
                //マニュアル操作
                OnestepTurn(2, mGear3Speed, mGear3Reverse);
            }
        }
    }
    
}

void HakoniwaGearMove::onPanelChanged(ofxUIEventArgs& e){
    const string name = e.widget->getName();

    if (name == "all gear turn") {
        if (mAllTurn != true) {
            cout << "mturn off " << endl;
            AllstepTurnStop();
        }
    }else if(name == "gear1"){
        if (mGear1 != true) {
            cout << "gear1 off " << endl;
            OnestepTurnStop(0);
        }
    }else if(name == "gear2"){
        if (mGear2 != true) {
            cout << "gear2 off " << endl;
            OnestepTurnStop(1);
        }
    }else if(name == "gear3"){
        if (mGear3 != true) {
            cout << "gear3 off " << endl;
            OnestepTurnStop(2);
        }
    }
    
    if(name == "allturn reverse"){
        if (mAllTurn == true) {
            AllstepTurn(mAllGearSpeed, mAllGearReverse);
        }
    }
    if(name == "gear1 reverse"){
        if (mGear1 == true) {
            OnestepTurn(0, mGear1Speed, mGear1Reverse);
        }
    }
    if(name == "gear2 reverse"){
        if (mGear2 == true) {
            OnestepTurn(1, mGear2Speed, mGear2Reverse);
        }
    }
    if(name == "gear3 reverse"){
        if (mGear3 == true) {
            OnestepTurn(2, mGear3Speed, mGear3Reverse);
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
    
    int mMotionExtCnt = 0;
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        ofPushMatrix();
        if (mMotionExtCnt >= 12 and mMotionExtCnt < 16) {
            int j = i -12;
            ofTranslate(1000, j*75);
        }else if(mMotionExtCnt >= 16 and mMotionExtCnt < 28){
            int k = i -16;
            ofTranslate(1200, k*75);
        }else if(mMotionExtCnt >= 28 and mMotionExtCnt < 32){
            int l = i -28;
            ofTranslate(1400, l*75);
        }else if(mMotionExtCnt >= 32 and mMotionExtCnt < 44){
            int m = i -32;
            ofTranslate(1600, m*75);
        }else if(mMotionExtCnt >= 44){
            int n = i -44;
            ofTranslate(1800, n*75);
        }else{
            ofTranslate(800, i*75);
        }
        mMotionExtCnt++;
        
        
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