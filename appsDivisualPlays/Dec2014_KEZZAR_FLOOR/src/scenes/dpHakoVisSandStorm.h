//
//  HakoVisSandStorm.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/05.
//
//

#ifndef RAMDanceToolkit_HakoVisSandStorm_h
#define RAMDanceToolkit_HakoVisSandStorm_h

#include "dpBasicMap.h"

class dpHakoVisSandStorm : public ramBaseScene{
public:
    string getName() const { return "dpVisSandStorm"; }
    
	void setupControlPanel(){
        ramGetGUI().addSlider("fade", 0.0, 255.0, &mFade);
        ramGetGUI().addSlider("radScale", 0.0, 1.0, &mRadScale);
        ramGetGUI().addSlider("flowDrawScale", 0.0, 40.0, &mFlowDrawScale);
        
        ramGetGUI().addToggle("maskMode", &isMaskMode);
        ramGetGUI().addToggle("isDrawLine", &isDrawLine);
        ramGetGUI().addToggle("isCircle", &isCircle);
        ramGetGUI().addToggle("isColor", &isColor);
        
        int a = 0;
        ramGetGUI().addIntSlider("colorSpeed", 2, 120, &mColorSpeed);
        
    }
    void setup(){
        
        mFbo.allocate(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT,GL_RGBA32F);
      
        mFbo.begin();
        ofClear(0,0,0,0);
        mFbo.end();
        
        mTri.loadImage("images/tri.png");
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/SandStorm/vector/total");
        
        mColor.speed = 0.01;
        mColor.imSet(dpColor::MAIN_COLOR.r,dpColor::MAIN_COLOR.g,dpColor::MAIN_COLOR.b);
        
        mMap.setup();
    }
    
    void update(){
        if(ofGetFrameNum() % mColorSpeed == 0)mColorCounter++;
        
        if(mColorCounter % 2 == 0){
            mColor.set(dpColor::MAIN_COLOR.r,dpColor::MAIN_COLOR.g,dpColor::MAIN_COLOR.b);
        }else{
            mColor.set(dpColor::PALE_PINK_LIGHT.r,dpColor::PALE_PINK_LIGHT.g,dpColor::PALE_PINK_LIGHT.b);
        }
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if(m.getAddress() == "/dp/cameraUnit/SandStorm/vector/total"){
                mMeanVector.set(m.getArgAsFloat(0),
                                  m.getArgAsFloat(1));
            }
        }
    
        mColor.update();
        
    }
    
    void draw(){
        ofDisableDepthTest();
        mFbo.begin();
        ofPushMatrix();
        ofTranslate(SINGLE_SCREEN_WIDTH * 0.5, SINGLE_SCREEN_HEIGHT * 0.5);
        ofPoint tmp = mMeanVector;//(ofGetMouseX(),ofGetMouseY());//farneback.getAverageFlow();
   
        mMeanVector *= mFlowDrawScale;
        
        ofColor color;
        
        
        if(isColor){
            
            color.set(mColor.x,mColor.y,mColor.z);
            ofSetColor(color);
        
        }
        else ofSetColor(255, 255, 255);
        
        ofSetLineWidth(2);
        if(isDrawLine)ofLine(0,0,tmp.x,tmp.y);
        
        if(isCircle){
            ofFill();
            ofCircle(tmp,tmp.length() * mRadScale);
            
        }else{
            ofPushMatrix();
            ofTranslate(tmp.x, tmp.y);
            ofRotateZ(ofRadToDeg(atan2(-tmp.y,-tmp.x) - HALF_PI));
            ofTranslate(-tmp.x, -tmp.y);
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            mTri.draw(tmp,tmp.length() * mRadScale,tmp.length() * mRadScale);
            ofPopStyle();
            ofPopMatrix();
        }
        
        ofPopMatrix();
        
        ofSetColor(0,0,0,mFade);
        ofRect(0,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
        
        mFbo.end();
        
        ofSetColor(255,255,255);
        mFbo.draw(0,0);
        
        if(isMaskMode)mMap.draw(mFbo.getTextureReference());
    }
private:
    
    float mFade = 10.0;
    float mRadScale = 10.0;
    float mFlowDrawScale = 10.0;
    
    bool isMaskMode = false;
    bool isDrawLine = true;
    bool isCircle = false;
    bool isColor = false;
    
    ofImage mTri;
    dpBasicMap mMap;
    
    ofFbo mFbo;
    
    int mColorSpeed = 20;
    int mColorCounter = 0;
    
    ramOscReceiveTag mReceiver;
    
    ofPoint mMeanVector;
    KezSlidePoint mColor;
    //dpIceMapGrid mGrid;
};

#endif
