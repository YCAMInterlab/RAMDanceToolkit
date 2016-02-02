//
//  dpDancerCue.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/01.
//
//

#include "dpDancerCue.h"

void dpDancerCue::setup(){
    ramOscManager::instance().addReceiverTag(&mReceiver);
    mReceiver.addAddress("/dp/dancerCue");
    
    mDancerNum["ANDO"] = ANDO;
    mDancerNum["KOJIRI"] = KOJIRI;
    mDancerNum["MIYASHITA"] = MIYASHITA;
    mDancerNum["SHIMAJI"] = SHIMAJI;
    
    for(int i = 0; i < DANCER_NUM; i++){
        float width = (float)SINGLE_SCREEN_WIDTH / (float)DANCER_NUM;
        
        CueRect rect;
        mRects.push_back(rect);
        mRects.back().setup(ofPoint(width * i,0),
                            ofPoint(width,mHeight),
                            dancerNameToColor((DANCER_NAME)i));
    }
    
}
void dpDancerCue::receiveOsc(){
    while (mReceiver.hasWaitingMessages()) {
        
        ofxOscMessage m;
        mReceiver.getNextMessage(&m);
        
        if(m.getAddress() == "/dp/dancerCue"){
            
            if(mDancerNum.count(m.getArgAsString(0))){
                
                int idx = mDancerNum[m.getArgAsString(0)];
                
                mRects[idx].setEnable(m.getArgAsInt32(1));
            }
            
        }
    }
}

void dpDancerCue::update(){
    
    receiveOsc();
    
    for(auto &v:mRects){
        v.update();
    }
}

void dpDancerCue::draw(){
    
    ofPoint offset = SCREEN_POSITIONS[SCREEN_C];
    
    ofPushMatrix();
    ofTranslate(offset.x,offset.y);
    
    for(auto &v:mRects){
        v.draw();
    }
    
    ofPopMatrix();
    
}

///// Rect

void dpDancerCue::CueRect::setup(ofPoint pos, ofPoint size, ofColor color){
    mPos = pos;
    mSize = size;
    mColor = color;
}

void dpDancerCue::CueRect::update(){
    
    if(mIsEnable){
        float blink = sin(ofGetFrameNum() * mAlphaBlinkSpeed);
        mAlpha = ofMap(blink,-1.0,1.0,0.0,255.0);
    }
    
}

void dpDancerCue::CueRect::draw(){
    
    if(mIsEnable){
        
        ofPushStyle();
        ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha);
        ofRect(mPos.x,mPos.y,mSize.x,mSize.y);
        ofPopStyle();
        
    }
}

