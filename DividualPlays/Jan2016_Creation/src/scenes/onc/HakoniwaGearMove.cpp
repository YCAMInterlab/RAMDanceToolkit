//
//  HakoniwaGearMove.cpp
//  RAMDanceToolkit
//
//  Created by ycam on 2014/12/25.
//
//

#include "HakoniwaGearMove.h"

void HakoniwaGearMove::setupControlPanel(){
    
    mDatahow = false;
    mAllTurn = false;
    mManual = false;

    mGear = true;
    mAllGearReverse = true;

    mGearReverse = true;
    
    mMaxSpeed = 13000;
    mMinSpeed = 1500;

    mSpeedScale = 50;
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    
    panel->addToggle("isManualMode", &mManual);
    panel->addToggle("On", &mGear);
    panel->addToggle("Show Data", &mDatahow);
    panel->addToggle("All Turn", &mAllTurn);
    panel->addToggle("All Reverse", &mAllGearReverse);
    panel->addIntSlider("all gear Speed", 100, 35000, &mAllGearSpeed);
    panel->addToggle("isReverse", &mGearReverse);
    panel->addIntSlider("CurrentSpeed", 1500, 13000, &mGearSpeed);
    
    ofAddListener(panel->newGUIEvent, this, &HakoniwaGearMove::onPanelChanged);
    motionExtractor.setupControlPanel(this,ofPoint(340,30));
    motionExtractor.load("motionExt_HakoniwaGearMove.xml");
    
    mParameterChangeThresh = 10;
}

void HakoniwaGearMove::onPanelChanged(ofxUIEventArgs& e){
    const string name = e.widget->getName();
    
    if (name == "All Turn") {
        if (mAllTurn != true) {
            cout << "turn off " << endl;
            AllstepTurnStop();
        }
    }else if(name == "On"){
        if (mGear != true) {
            cout << "gear off " << endl;
            OnestepTurnStop(2);
        }
    }
    
    if(name == "All Reverse"){
        if (mAllTurn == true) {
            AllstepTurn(mAllGearSpeed, mAllGearReverse);
        }
    }
    
    if(name == "isReverse"){
        if (mGear == true) {
            OnestepTurn(2, mGearSpeed, mGearReverse);
        }
    }
    
}


void HakoniwaGearMove::setup(){
    
    stepManager.setupOsc(GEAR_IP, 8528);
	stepManager.sendByteSimply = true;

    stepManager.addStepper("unit1", 400, 0);
    stepManager.addStepper("unit2", 400, 1);
    stepManager.addStepper("unit3", 400, 2);
    stepManager.resetAllDevices();

    stepManager.setParam_maxSpeed(0x0075);
    stepManager.setParam_Accel(0x0010);
    stepManager.setParam_Decel(0x0010);
    stepManager.setMicroSteps(7);
    
	stepManager.setMicroSteps(5);
    
    stepManager.setStepperAll(true);
    stepManager.absPos(0);
    stepManager.hardStop();

}

void HakoniwaGearMove::update(){

    motionExtractor.update();
    
}

void HakoniwaGearMove::sumVelocitySpeed(){
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        mVelocitySpeedSum += motionExtractor.getVelocitySpeedAt(i);
    }
}

void HakoniwaGearMove::randomiseGearDirection(){
    if ( (int)ofRandom(0,2) == 0){
        mGearReverse = true;
    }else{
        mGearReverse = false;
    }
}

void HakoniwaGearMove::calcGearSpeed(){
    mGearSpeed = mVelocitySpeedSum * mSpeedScale;
    mGearSpeed = ofClamp(mGearSpeed, mMinSpeed, mMaxSpeed);
    cout << "GearSpeed " << mGearSpeed << endl;
}

void HakoniwaGearMove::reset(){
    mVelocitySpeedSum = 0;
    mParameterChangeCount = 0;
}

void HakoniwaGearMove::draw(){

    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。

    if (mDatahow)drawDump();

    if (ofGetFrameNum() % 30 == 0){
        
        sumVelocitySpeed();
        
        mParameterChangeCount++;
        
        if(mManual){
            
            if(mGear){
                
                //マニュアル操作
                OnestepTurn(2, mGearSpeed, mGearReverse);
                
            }
            
        }else{
            
            if (mParameterChangeCount >= mParameterChangeThresh) {
                
                calcGearSpeed();
                
                if(mGear){
                    
                    randomiseGearDirection();
                    OnestepTurn(2, mGearSpeed, mGearReverse);
                    
                }
                
                reset();
                
            }
            
        }
        
        if(mAllTurn){
            stepManager.setStepperAll(true);
            stepManager.run(mAllGearSpeed, mAllGearReverse);
            stepManager.setStepperAll(false);
        }
    }
}

void HakoniwaGearMove::onDisabled(){
    
    cout << "ondisebled " << endl;
    stepManager.setStepperAll(true);
    stepManager.softStop();

}

void HakoniwaGearMove::AllstepTurn(int speed, bool dir){
    
    float currentSpeed = ofClamp(speed, mMinSpeed, mMaxSpeed);
    
    stepManager.setStepperAll(true);
    stepManager.run(currentSpeed, dir);
    stepManager.setStepperAll(false);

}

void HakoniwaGearMove::OnestepTurn(int ch, int speed, bool dir){
    
    float currentSpeed = ofClamp(speed, mMinSpeed, mMaxSpeed);
    
    stepManager.selectStepperOne(ch, true);
    stepManager.run(currentSpeed, dir);
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
    
    for (int i = 0;i < motionExtractor.getNumPort();i++){
        
        ofPushMatrix();
        
        if (i >= 12 && i < 16) {
            int j = i - 12;
            ofTranslate(1000, j*75);
        }else if(i >= 16 && i < 28){
            int k = i - 16;
            ofTranslate(1200, k*75);
        }else if(i >= 28 && i < 32){
            int l = i - 28;
            ofTranslate(1400, l*75);
        }else if(i >= 32 && i < 44){
            int m = i - 32;
            ofTranslate(1600, m*75);
        }else if(i >= 44){
            int n = i - 44;
            ofTranslate(1800, n*75);
        }else{
            ofTranslate(800, i*75);
        }
        
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