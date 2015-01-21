//
//  dpHakoVisRecordRibbon.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_dpHakoVisRecordRibbon_h
#define RAMDanceToolkit_dpHakoVisRecordRibbon_h

#include "dpRibbon.h"

class dpRibbonControler{
public:

    dpRibbon<2000> mRibbon;
    float mTargetPosZ = 0;
    KezSlidePoint mTarget;
    
    ofColor mColor;
    bool isFocus = false;
    
    void setup(){
        mRibbon.setup();
        mRibbon.setWidth(0.1, 0.5);
        mTarget.speed = 0.1;
    }
    
    void setColor(ofColor color){
        mColor = color;
    }
    
    void setTarget(ofPoint pt){
        mTarget.set(pt);
    }
    
    void setForcus(bool focus){
        isFocus = focus;
    }
    
    void update(){
        if(isFocus){
            mRibbon.setHead(mTarget);
            mTarget.update();
            mRibbon.update();
        }
    }
    
    void draw(){
        ofSetColor(mColor);
        mRibbon.draw();
        
        const ofPoint &head = mRibbon.getHead();
    
        ofDrawSphere(head, 0.5);
    }
};

class dpSPBox{
public:
    
    void start(ofPoint size,float angle){
        mSize = size;
        mAngle = angle;
        mColor = dpColor::MAIN_COLOR;
        mSat.imSet(255);
        mSat.set(0);
        mSat.speed = 0.03;
    }
    
    void update(){
        mPos.z += mSpeed;
        mSat.update();
        
        if(mSat.val > 1)mColor.setSaturation(mSat.val);
    }
    
    void draw(){
    
        ofSetColor(mColor.r,mColor.g,mColor.b,ofMap(mPos.z,230,300,150,0,true));
        
        ofPushMatrix();
        ofTranslate(0, 0, mPos.z);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawBox(0,0,0,mSize.x,mSize.y,mSize.z);
        ofPopMatrix();
    
        ofPushMatrix();
        ofTranslate(0, 0, -mPos.z);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawBox(0,0,0,mSize.x,mSize.y,mSize.z);
        ofPopMatrix();
       
    }
    
private:
    ofPoint mPos;
    float mSpeed = 1.0;
    float mRad;
    int mRes = 3;
    ofPoint mSize;
    float mAngle = 0.0;
    bool isPink = false;
    ofColor mColor;
    KezSlide mSat;
};

class dpHakoVisServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpVisServoPendulum"; }
    
	void setupControlPanel(){
        
        mScale.set(200,100);
        
        ramGetGUI().addSlider("scaleX", 100.0, 2000.0, &mScale.x);
        ramGetGUI().addSlider("scaleY", 1.0, 2000.0, &mScale.y);
        ramGetGUI().addButton("rndOrbit");

        mLong.speed = 0.001;
        mLat.speed = 0.001;
        mRad.speed = 0.01;
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisServoPendulum::onPanelChanged);
    }
    void setup(){
    
     
        mCam.setVFlip(true);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/ServoPendulum/vector/total");
        
        mHead.speed = 0.2;

    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/ServoPendulum/vector/total"){
                mBoxes.push_back(dpSPBox());
                
                float pAngle = atan2(mHead.y,mHead.x);
                
                mHead.set(m.getArgAsFloat(0),
                          m.getArgAsFloat(1));
                
                ofPoint tmp(mHead.x,mHead.y);
                
   //             mHead = tmp;
   
                float angle = atan2(tmp.y,tmp.x);
          
                
                mAngle = angle;
                
                mBoxes.back().start(ofPoint(1.0,tmp.length() * mScale.x,1.0),angle);
                if(mBoxes.size() > BOX_MAX)mBoxes.pop_front();
            }
        
        }
    }
    
    void update(){
        
        receiveOsc();
   
        for(auto &v:mBoxes){
            v.update();
        }
        
        mHead.update();
        
        mLong.update();
        mLat.update();
        mRad.update();
        
        mCam.lookAt(ofPoint(0,0,0));
        mCam.orbit(mLong.val, mLat.val, mRad.val);
        
        if(ofGetFrameNum() % 600 == 0)rndOrbit();
    }
    
    void rndOrbit(){
        mLong.set(ofRandom(-180,180));
        mLat.set(ofRandom(-180,180));
        mRad.set(ofRandom(0,400));
    }
    
    
    
    void draw(){

        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
     
        mCam.begin(dpGetFirstScreenViewPort());
        
        ofSetColor(dpColor::MAIN_COLOR.r,
                   dpColor::MAIN_COLOR.g,
                   dpColor::MAIN_COLOR.b,
                   150);
        
        ofPushMatrix();
        ofTranslate(0, 0, 0.0);
        ofRotateZ(ofRadToDeg(mAngle));
        ofDrawBox(0,0,0,1.0,mHead.length() * mScale.x,1.0);
        ofPopMatrix();
        
        ofNoFill();
        for(auto &v:mBoxes){
            v.draw();
        }
        mCam.end();
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
        if(name == "rndOrbit")rndOrbit();
    }
    
private:
    
    ofEasyCam mCam;

    ramOscReceiveTag mReceiver;
    
    ofPoint mScale;
    
    deque<ofPoint>mPts;
    static const int BOX_MAX = 300;
    deque<dpSPBox>mBoxes;
    ofPoint mLightRot;
    KezSlidePoint mHead;
    
    float mAngle = 0.0;
    
    ofPoint mRot;
    
    KezSlide mLong;
    KezSlide mLat;
    KezSlide mRad;
    
    
};

#endif
