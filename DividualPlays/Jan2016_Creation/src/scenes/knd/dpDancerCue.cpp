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
                
                mRects[idx].setEnable(m.getArgAsInt32(1),ofGetElapsedTimef());
            }
            
        }
    }
}

void dpDancerCue::update(){
    
    receiveOsc();
    
    float time = ofGetElapsedTimef();
    
    for(auto &v:mRects){
        v.update(time);
    }
    
}

void dpDancerCue::draw(){
    
    ofPushStyle();
    
    ofPushMatrix();
    
    ofPoint offset = SCREEN_POSITIONS[SCREEN_C];
    ofTranslate(offset.x,offset.y);
    
    for(auto &v:mRects){
        v.draw();
    }
    
    ofPopMatrix();
   
    for(auto &v:mRects){
        v.draw();
    }
    
    ofPopStyle();
}

///// Rect

void dpDancerCue::CueRect::setup(ofPoint pos, ofPoint size, ofColor color){
    mPos = pos;
    mSize = size;
    mColor = color;
}

void dpDancerCue::CueRect::update(float time){
    
    if(mIsEnable){
        float blink = sin((time - mElapsed) * mAlphaBlinkSpeed);
        mAlpha = ofMap(blink,-1.0,1.0,0.0,255.0);
    }
    
    mAlphaScale.update();
    
}

void dpDancerCue::CueRect::setEnable(bool enable, float time){

    mIsEnable = enable;
    
    mElapsed = time;
    
    if(mIsEnable){
        mAlphaScale.imSet(0.0);
        mAlphaScale.set(1.0);
    }else{
        mAlphaScale.set(0.0);
    }
}

void dpDancerCue::CueRect::draw(){
    
    if(mAlphaScale.val > 0.01){
        
        ofPushStyle();
        ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha * mAlphaScale.val);
        ofRect(mPos.x,mPos.y,mSize.x,mSize.y);
        ofPopStyle();
        
    }
}

