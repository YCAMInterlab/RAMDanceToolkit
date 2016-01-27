//
//  dpVisMemeFadeRect.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/25.
//
//

#ifndef RAMDanceToolkit_dpVisMemeFadeRect_h
#define RAMDanceToolkit_dpVisMemeFadeRect_h

#include "MemeConstants.h"
#include "KezSlidePoint.h"

class KezFadeRect{
public:
    
    void fire(ofColor color, ofPoint size){
        mColor = color;
        mSize.imSetX(size.x);
        mSize.imSetY(0);
        mSize.setY(size.y);
    
        mAlpha = 255.0;
        
        mPos.set(ofRandom(0,SINGLE_SCREEN_WIDTH),
                 ofRandom(0,SINGLE_SCREEN_HEIGHT));
        
        mFadeSpeed = 1.0;
        mSize.speed = 0.333;
    }
    
    void reset(){
        mAlpha = 0.0;
    }
    
    void update(){
        mSize.update();
        mAlpha -= mFadeSpeed;
    }
    
    void draw(){
        if(mAlpha > 1.0){
            ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha);
            ofFill();
            ofRect(mPos.x,mPos.y,
                   mSize.x,mSize.y);
            
            ofSetLineWidth(2);
            ofNoFill();
            ofSetColor(mColor.r,mColor.g,mColor.b,mAlpha);
            ofRect(mPos.x,mPos.y,
                   mSize.x,mSize.y);
        }
    }
private:
    ofPoint mPos;

    ofColor mColor;
    float mAlpha;
    float mFadeSpeed = 1.0;
    
    KezSlidePoint mSize;
};

class KezFadeRectManager{
public:
    void setup(){
        for(int i = 0; i < NUM; i++){
            mRects.push_back(KezFadeRect());
        }
    }
    
    void fire(ofColor color, ofPoint size){
        mRects[mCounter].fire(color,size);
        mCounter++;
        mCounter %= NUM;
    }
    
    void update(){
        for(auto &v:mRects){
            v.update();
        }
    }
    
    void reset(){
        for(auto &v:mRects){
            v.reset();
        }
    }
    
    void draw(){
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        for(auto &v:mRects){
            v.draw();
        }
        ofPopStyle();
    }
private:
    static const int NUM = 200;
    vector<KezFadeRect>mRects;
    int mCounter = 0;
};

class dpVisMemeFadeRect : public ramBaseScene{
public:
    
    string getName() const { return "dpVisMemeFadeRect"; }
    
    void setupControlPanel(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/meme");
    }
    
    void setup(){
        mRectManager.setup();
    }
    
    void update(){
        mRectManager.update();
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
                                    
                                    mRectManager.fire(color,ofPoint(param * 200.0));
                                }
                        }
                    }
                }
            }
        }
    }
    
    void onEnabled(){
        mRectManager.reset();
        mIsReceiveOsc = false;
    }
    
    void draw(){
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        mRectManager.draw();
    }
private:
    
    KezFadeRectManager mRectManager;
    ramOscReceiveTag mReceiver;
    
    bool mIsReceiveOsc = true;
};

#endif
