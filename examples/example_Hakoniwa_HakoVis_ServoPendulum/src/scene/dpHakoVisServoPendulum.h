//
//  HakoniwaServoPendulum.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_HakoVisServoPendulum_h
#define RAMDanceToolkit_HakoVisServoPendulum_h

#include "ramMotionExtractor.h"

template <int Z_MAX = 600>
class dpSPBox{
public:
    
    dpSPBox(){
		mSpeed = 1.0;
		mAngle = 0.0;

		mSatDec = 10.0;

        mColor.set(255, 50, 150);
    }
    
    void start(ofPoint size,float angle){
        mSize = size;
        mAngle = angle;
        mSat = 255;
    }
    
    void update(){
        mPos.z += mSpeed;        
        if(mSat> 1){
            mColor.setSaturation(mSat);
            mColor.setBrightness(ofMap(mSat,255,0,255,90));
        }
        
        mSat -= mSatDec;
    }
    
    void draw(){
        
        ofPushStyle();
        ofNoFill();
        ofSetColor(mColor.r,mColor.g,mColor.b,ofMap(mPos.z,230,Z_MAX,255,0,true));
        
        ofPushMatrix();
        ofTranslate(0, 0, mPos.z);
      //  ofRotateY(90);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(0,0,0,mSize.x,mSize.y);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(0, 0, -mPos.z);
        //ofRotateY(90);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetRectMode(OF_RECTMODE_CENTER);
         ofRect(0,0,0,mSize.x,mSize.y);
        ofPopMatrix();
        
        ofPopStyle();
    }
    
private:
    
    ofPoint mPos;
    ofPoint mSize;
    
    ofColor mColor;
    
	float mSpeed;
	float mAngle;
    
    float mSat;
	float mSatDec;
};

class dpHakoVisServoPendulum : public ramBaseScene{
public:
    
    string getName() const { return "dpVisServoPendulum"; }
    
	void setupControlPanel(){
        
        mScale = 600;
        
        ramGetGUI().addSlider("scale", 100.0, 2000.0, &mScale);
        
        mColor.set(255, 50, 150);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisServoPendulum::onPanelChanged);
    }
    
    void setup(){
		mAngle = 0.0;
        mCam.setPosition(ofPoint(1, 0, 0));
        mCam.setTarget(ofPoint(0, 0, 0));
        mCam.setDistance(300);
        mCam.setVFlip(true);
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/ServoPendulum/vector/total");
   
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/cameraUnit/ServoPendulum/vector/total"){
                
                mBoxes.push_back(dpSPBox<RECT_MAX>());
                
                mHead.set(m.getArgAsFloat(0), m.getArgAsFloat(1));
            
                mAngle = atan2(mHead.y, mHead.x);
                
                mBoxes.back().start(ofPoint(1.0, mHead.length() * mScale, 1.0), mAngle);
                
                if(mBoxes.size() > RECT_MAX)mBoxes.pop_front();
                
            }

        }
    }
    
    void update(){
        
        receiveOsc();
    
        for(auto &v:mBoxes){
            v.update();
        }
    
    }
    
    void draw(){
        
        ofDisableDepthTest();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetLineWidth(2);
        
        mCam.begin();
        
        ofPushMatrix();
        ofTranslate(0, 0, 0);
        ofRotateZ(ofRadToDeg(mAngle));
        ofSetColor(mColor);
        ofDrawBox(0, 0, 0, 1.0, mHead.length() * mScale, 1.0);
        ofPopMatrix();
        
        for(auto &v:mBoxes){
            v.draw();
        }
        
        mCam.end();
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
    }
    
private:
    
    ofEasyCam mCam;
    
    ramOscReceiveTag mReceiver;
    
    float mScale;
	float mAngle;
    
    static const int RECT_MAX = 600;
    deque<ofPoint>mPts;
    deque<dpSPBox<RECT_MAX> >mBoxes;
    
    ofPoint mHead;
    ofPoint mCamPos;
    
    static const int RND_ROT_FRAME = 1200;
    
    ofColor mColor;
};


#endif
