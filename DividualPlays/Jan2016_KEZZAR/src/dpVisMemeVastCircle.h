//
//  dpVisMemeCircle.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/25.
//
//

#ifndef RAMDanceToolkit_dpVisMemeCircle_h
#define RAMDanceToolkit_dpVisMemeCircle_h

#include "MemeConstants.h"

class KezVastCircle{
public:
    
    void fire(ofColor color, float rad){
        mColor = color;
    
        mAlpha = 255.0;
        
        mPos.set(0,0);
        
        mFadeSpeed = 1.0;
        
        mRad.imSet(0);
        mRad.set(rad);
        
    }
    
    void update(){
        mRad.update();
        mAlpha -= mFadeSpeed;
    }
    
    void draw(){
        if(mAlpha > 1.0){
            
            ofPushStyle();
            ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha);
            ofNoFill();
            ofCircle(mPos,mRad.val);
            
            ofFill();
            ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha * 0.25);
            ofCircle(mPos,mRad.val);
            ofPopStyle();
            
        }
    }
    
    void reset(){
        mAlpha = 0.0;
    }
    
private:
    ofPoint mPos;
    
    ofColor mColor;
    float mAlpha;
    float mFadeSpeed = 1.0;
    
    KezSlide mRad;
};

class KezVastCircleManager{
public:
    
    void setup(ofPoint pos){
        for(int i = 0; i < NUM; i++){
            mCircles.push_back(KezVastCircle());
        }
        
        mPos = pos;
    }
    
    void fire(ofColor color, float rad){
        mCircles[mCounter].fire(color,rad);
        mCounter++;
        mCounter %= NUM;
    }
    
    void update(){
        for(auto &v:mCircles){
            v.update();
        }
    }
    
    void reset(){
        for(auto &v:mCircles){
            v.reset();
        }
    }
    
    void draw(){
        
        ofPushMatrix();
        ofTranslate(mPos.x, mPos.y);
        
        ofPushStyle();

        for(auto &v:mCircles){
            v.draw();
        }
        ofPopStyle();
        
        ofPopMatrix();
    }
private:
    static const int NUM = 200;
    vector<KezVastCircle>mCircles;
    int mCounter = 0;
    ofPoint mPos;
};

class dpVisMemeVastCircle : public ramBaseScene{
public:
    
    string getName() const { return "dpVisMemeCircle"; }
    
    void setupControlPanel(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/meme");
    }
    
    void setup(){
        
        for(int i = 0; i < MEME_NUM; i++){
            float x = SINGLE_SCREEN_WIDTH / (float)MEME_NUM;
            
            mCircleManager.push_back(KezVastCircleManager());
            mCircleManager.back().setup(ofPoint(x * i - (x * MEME_NUM) * 0.5 + x * 0.5 + SINGLE_SCREEN_WIDTH * 0.5,
                                                SINGLE_SCREEN_HEIGHT * 0.5));
        }
        
    }
    
    void update(){
        
        for(auto &v:mCircleManager){
            v.update();
        }

        receiveOsc();
        
        mIsReceiveOsc = true;
        
    }
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/toVis/meme"){
                if(mIsReceiveOsc){
                    for(int i = 0; i < MEME_NUM; i++){
                        int idx = m.getArgAsInt32(0);
                        if(idx == i){
                            
                            float speed = m.getArgAsFloat(1);
                            if(speed > 0.0){
                                float param = ofMap(speed,MIN_SPEED,MAX_SPEED,1.0,0.0);
                                ofColor color = dpColor::MAIN_COLOR;
                                color.setSaturation(param * color.getSaturation());
                                
                                mCircleManager[i].fire(color, param * 400.0);
                                
                            }
                        }
                    }
                }
            }
        }
    }
    
    void onEnabled(){
        for(auto &v:mCircleManager){
            v.reset();
        }
        
        mIsReceiveOsc = false;
    }
    
    void draw(){
        ofSetCircleResolution(60);
        ofSetLineWidth(3);
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        for(auto &v:mCircleManager){
            v.draw();
        }
        
    }
private:
    
    vector<KezVastCircleManager> mCircleManager;
    ramOscReceiveTag mReceiver;
    
    bool mIsReceiveOsc = false;
    
};

#endif
