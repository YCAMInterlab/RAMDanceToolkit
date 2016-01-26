//
//  dpVisMemeBox.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/25.
//
//

#ifndef RAMDanceToolkit_dpVisMemeBox_h
#define RAMDanceToolkit_dpVisMemeBox_h

#include "dpConstants.h"
#include "MemeConstants.h"
#include "KezSlidePoint.h"

class dpMemeBox{
public:
    
    void fire(ofColor color,ofPoint size){
        mColor = color;
        mPos.set(0,0,0);
        
        mSize.imSet(10,2,2);
        mSize.set(size);
        mSize.speed = 0.333;
        
        mIsEnable = true;
    }
    
    void update(){
        mPos.z += mSpeed;
        mSize.update();
    }
    void draw(){
        if(mIsEnable){
            ofSetColor(mColor);
            ofDrawBox(mPos.x,mPos.y,mPos.z,
                      mSize.x,mSize.y,mSize.z);
        }
    }
private:
    ofPoint mPos;
    KezSlidePoint mSize;
    float mSpeed = -6.0;
    ofColor mColor;
    
    bool mIsEnable = false;
};

class dpMemeBoxManager{
public:
    void setup(ofPoint pos){
        for(int i = 0; i < MAX_BOX; i++){
            mBoxes.push_back(dpMemeBox());
        }
        mPos = pos;
    }
    
    void fire(ofColor color, ofPoint size){
        
        mNextColor = color;
        mNextSize = size;
        
    }
    
    void next(){
        mBoxes[mCounter].fire(mNextColor,mNextSize);
        mCounter++;
        mCounter %= MAX_BOX;
    }
    
    void update(){
        for(auto &v:mBoxes){
            v.update();
        }
        
        next();
        
        reset();
    }
    
    void reset(){
        mNextColor.set(255,150);
        mNextSize.set(10,2,2);
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(mPos);
        for(auto &v:mBoxes){
            v.draw();
        }
        ofPopStyle();
        ofPopMatrix();
    }
private:
    int mCounter = 0;
    static const int MAX_BOX = 300;
    
    vector<dpMemeBox>mBoxes;
    
    ofPoint mPos;
    
    ofColor mNextColor;
    ofPoint mNextSize;
};

class dpVisMemeBox : public ramBaseScene{
public:
    
    string getName() const { return "dpVisMemeBox"; }
    
    void setupControlPanel(){
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/meme");
    }
    
    void setup(){
        for(int i = 0; i < MEME_NUM; i++){
            
            float x = SINGLE_SCREEN_WIDTH / (float)MEME_NUM;
    
            mBoxManagers.push_back(dpMemeBoxManager());
            mBoxManagers.back().setup(ofPoint(x * i - (x * MEME_NUM) * 0.5 + x * 0.5,0.0));
            
        }
    }
    
    void update(){
        for(auto &v:mBoxManagers){
            v.update();
        }
        
        if(bEnabled)receiveOsc();
        
    }
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/toVis/meme"){
                for(int i = 0; i < MEME_NUM; i++){
                    int idx = m.getArgAsInt32(0);
                    if(idx == i){
                        float speed = m.getArgAsFloat(1);
                        
                        if(speed > 0.0){
                            
                            float param = ofMap(speed,MIN_SPEED,MAX_SPEED,1.0,0.0);
                            ofColor color = dpColor::MAIN_COLOR;
                            color.setSaturation(param * color.getSaturation());
                            
                            mBoxManagers[i].fire(color,ofPoint(param * 700.0,10,10));
                            
                        }
                    }
                }
            }
        }
    }
    
    void draw(){
        
        ofSetBoxResolution(1);
        
        mCam.begin(dpGetFirstScreenViewPort());
        
        ofPushMatrix();
        ofTranslate(0,0,POS_OFFSET_Z);
        for(auto &v:mBoxManagers){
            v.draw();
        }
        ofPopMatrix();
        
        mCam.end();
        
    }
    
private:
    
    ramOscReceiveTag mReceiver;

    vector<dpMemeBoxManager>mBoxManagers;
    
    ofEasyCam mCam;
    
    static const int POS_OFFSET_Z = 400;
};

#endif
