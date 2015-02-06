//
//  dpRecordBufferCircle.cpp
//  example_hakoniwa_hakoVis
//
//  Created by kezzardrix2 on 2015/01/30.
//
//

#include "dpRecordBufferCircle.h"

const ofColor dpRecordBufferCircle::MAIN_COLOR = ofColor(255,50,150);

void dpRecordBufferCircle::setup(ofPoint origin){
    
    mOrigin = origin;
    
    mBufs.clear();
    
    for(int i = 0; i < BUFFER_MAX; i++){
        mBufs.push_back(mOrigin);
    }
    
    mPos.set(mOrigin);
    mTargetPos.set(mOrigin);
   
    mHue = MAIN_COLOR.getHue();
    mSaturation = MAIN_COLOR.getSaturation();
    mBrightness = MAIN_COLOR.getBrightness();
    
    mCurrentColor.setHsb(mHue,mSaturation,mBrightness);
    
}

void dpRecordBufferCircle::record(ofPoint pt){
    
    mTargetPos.set(pt + mOrigin);
    mBufs.push_back(pt + mOrigin);
    isRecordOnce = true;
    
}

void dpRecordBufferCircle::startRecord(){
    
    mBufs.clear();
    isRecord = true;
    
}

void dpRecordBufferCircle::startPlayback(){
    
    isRecord = false;
    mPlaybackCounter = 0;
    
}

void dpRecordBufferCircle::swapPts(ofPoint pt){
    
    mBufs[0] = pt;
    
    for(int i = 1; i < BUFFER_MAX; i++){
        ofPoint tmp = mBufs[i-1];
        mBufs[i - 1] = mBufs[i];
        mBufs[i] = tmp;
    }
}

void dpRecordBufferCircle::updateAlpha(){
    
    if(!isRecord)mRecordAlpha -= mRecordAlphaSpeed;
    else mRecordAlpha += mRecordAlphaSpeed;
    
    mRecordAlpha = fmaxf(mRecordAlpha, 0.0);
    mRecordAlpha = fminf(mRecordAlpha, 255.0);
    
}

void dpRecordBufferCircle::updateSaturation(){
    
    mCurrentColor.setHue(mHue);
    mCurrentColor.setSaturation((int)(mRecordAlpha / 255.0 * mSaturation));
    mCurrentColor.setBrightness(mBrightness);
    
}

void dpRecordBufferCircle::update(){
    
    if(mBufs.empty() == false && isRecord == false && isRecordOnce){
        
        mTargetPos.set(mBufs[mPlaybackCounter]);
        mPlaybackCounter++;
        mPlaybackCounter %= mBufs.size();
        
    }
    
    updateAlpha();
    updateSaturation();
    
    mPos += (mTargetPos - mPos) * mPosEase;
    
}

void dpRecordBufferCircle::draw(){
    
    ofPushStyle();
    
    ofFill();
    ofSetColor(mCurrentColor);
    ofCircle(mPos.x,mPos.y,mRad);
    
    ofSetColor(mCurrentColor,mRecordAlpha);
    ofNoFill();
    ofCircle(mPos.x,mPos.y,mRad * mRecordCircleScale);
    
    ofPopStyle();
    
}

ofPoint dpRecordBufferCircle::getPos(){
    return mPos;
}


