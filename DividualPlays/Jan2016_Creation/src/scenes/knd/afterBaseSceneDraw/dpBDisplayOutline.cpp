//
//  dpBDisplayOutline.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/03.
//
//

#include "dpBDisplayOutline.h"

void dpBDisplayOutline::setup(){
    
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/BDisplayOutline/enable");
    mReceiver.addAddress("/dp/BDisplayOutline/width");
    
    mIsEnable = false;
    mLineWidth = 1.0;
    
};

void dpBDisplayOutline::update(){
    receiveOsc();
};

void dpBDisplayOutline::receiveOsc(){
    while (mReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/dp/BDisplayOutline/enable"){
            mIsEnable = m.getArgAsInt32(0);
        }
        
        if(m.getAddress() == "/dp/BDisplayOutline/width"){
            mLineWidth = m.getArgAsInt32(0);
        }
    }
};
void dpBDisplayOutline::draw(){
    
    if(mIsEnable){
        
        ofPoint offset = SCREEN_POSITIONS[SCREEN_B];
        
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(offset.x, offset.y);
        ofNoFill();
        ofSetLineWidth(mLineWidth);
        ofSetColor(255);
        ofRect(0,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
        ofPopMatrix();
        ofPopStyle();
        
    }
};