//
//  dpHakoniwaStruggle.cpp
//  example-ramMotionExtractor
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#include "dpHakoniwaStruggle.h"

void dpHakoniwaStruggle::setupControlPanel(){
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    ramGetGUI().addSlider("scaleVelocitySpeed", 1.0, 100.0, &mVelocitySpeedScale);
    
}

void dpHakoniwaStruggle::setup(){
 
    mSender.setup("192.168.20.",8528);
}

void dpHakoniwaStruggle::sendOsc(){
    
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/struggle");
    m.addIntArg(0); //モーターの回転方向
    m.addIntArg(1); //モーターの回転方向
    m.addIntArg(mVelocitySpeed); //モーターの強さ
    mSender.sendMessage(m);
    
}

void dpHakoniwaStruggle::update(){

    mMotionExtractor.update();
    mVelocitySpeed = mMotionExtractor.getVelocitySpeedAt(0) * mVelocitySpeedScale;
    
}

void dpHakoniwaStruggle::drawActor(const ramActor &actor){
    ramDrawBasicActor(actor);
}

void dpHakoniwaStruggle::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    
    ofPoint nodePos = mMotionExtractor.getPositionAt(0);
    
    ofPoint boxPos(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));
    boxPos *= mVelocitySpeed * 0.1;
   
    ofSetColor(255,255);
    ofLine(boxPos,nodePos);
    ofLine(boxPos,ofPoint(0,1000,0));
    ofDrawBox(boxPos,30,30,30);
    
    ramEndCamera();
}