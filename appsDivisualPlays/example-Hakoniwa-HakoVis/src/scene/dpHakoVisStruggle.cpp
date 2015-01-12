//
//  dpHakoVisStruggle.cpp
//  example-ramMotionExtractor
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#include "dpHakoVisStruggle.h"

void dpHakoVisStruggle::setupControlPanel(){
    ramGetGUI().addSlider("scale",1.0,20.0,&mScale);
}

void dpHakoVisStruggle::setup(){
    
    ramOscManager::instance().addReceiverTag(&mReceiver); //oscの受信はramOscReceiveTag
    mReceiver.addAddress("/dp/cameraUnit/struggle/vector/total");
    
}

void dpHakoVisStruggle::receiveOsc(){
    
    while(mReceiver.hasWaitingMessages()){
        
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
       
        if(m.getAddress() == "/dp/cameraUnit/struggle/vector/total"){
            mTotalVec.set(m.getArgAsFloat(0),m.getArgAsFloat(1));
        }
    }
}

void dpHakoVisStruggle::update(){
    receiveOsc();
}

void dpHakoVisStruggle::draw(){
    
    ofPoint head = mTotalVec;
    head *= mScale;
    
    ramSetViewPort(dpGetSecondScreenViewPort()); //2枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    ofSetColor(255,255,255);
    ofLine(0,0,head.x,head.y);
    ofCircle(head,40);
    ramEndCamera();
    
    ramSetViewPort(dpGetFirstScreenViewPort());
}