//
//  dpHakoniwaServoPendulum.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/08.
//
//

#ifndef RAMDanceToolkit_dpHakoniwaServoPendulum_h
#define RAMDanceToolkit_dpHakoniwaServoPendulum_h

#include "dpServoPendulumThread.h"
#include "ramMotionExtractor.h"
#include "KezSlidePoint.h"
#include "dpConstants.h"

class dpHakoniwaServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpHServoPendulum"; }
	void setupControlPanel(){
        
        ramGetGUI().addSlider("speed", 0.1, 4.0, &mSpeed);
        ramGetGUI().addSlider("speedDefault", 1.0, 10.0, &mSpeedDefault);
        ramGetGUI().addSlider("range", 0.0, 90, &mRange);
        ramGetGUI().addSlider("rad", 5, 300, &mRad);
        ramGetGUI().addSlider("velScale", 0.01, 10.0, &mVelScale);
        ramGetGUI().addSlider("limit", 0.1, 1.0, &mLimit);
        ramGetGUI().addSlider("scale", 0.01, 10.0, &mScale);
        ramGetGUI().addToggle("isRotMode",&isRotMode);
        
        mMotionExtractor.setupControlPanel(this);
        mMotionExtractor.load("motionExt_dpHServoPendulum.xml");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaServoPendulum::onPanelChanged);
    }
    void setup(){
        
        mThread.setup();
        
        mSender.setup("192.168.20.67",8528);
    }
    
    void setRange(float range){
        mRange = range;
        mRange = fmaxf(0, mRange);
        mRange = fminf(90,mRange);
        
        mThread.setRange(mRange);
    }
    
    void update(){
        
        
        float shake = 0.0;//mMotionExtractor.getVelocityAt(0).x * mVelScale;
        
        if(isRotMode){
            
            if(mMotionExtractor.getIsExist(0)){
            
                    ofNode node = mMotionExtractor.getNodeAt(0);
                
                    if(node.getParent() != NULL){
                        ofQuaternion quat = node.getParent()->getGlobalOrientation().inverse() * node.getGlobalOrientation();
              
                        float angle;
                        ofPoint axis;
                        quat.getRotate(angle, axis);
                     
                        mRange = angle * mScale;
                        setRange(mRange);
                    }
                }
            }else{
            
                ofPoint pos1 = mMotionExtractor.getPositionAt(0);
                ofPoint pos2 = mMotionExtractor.getPositionAt(1);
            
                setRange((pos1 - pos2).length() * mScale);
            
            }
        
        mThread.setSpeed(mSpeed);
        
        mMotionExtractor.update();
        mAngle.update();
    }
    
    void draw(){
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        
        ramBeginCamera();
        
        if(isRotMode){
     
            ofNode node = mMotionExtractor.getNodeAt(0);
                
            if(node.getParent() != NULL){
                ofQuaternion quat = node.getParent()->getGlobalOrientation().inverse() * node.getGlobalOrientation();
                
                float angle;
                ofPoint axis;
                quat.getRotate(angle, axis);
                
                mRange = angle * mScale;
                setRange(mRange);
            
                ofPoint pos = node.getGlobalPosition();
                
                ofPushMatrix();
                ofTranslate(pos);
                ofRotate(angle, axis.x, axis.y, axis.z);
                ofPushStyle();
                ofNoFill();
                ofDrawBox(0,0,0,30,30,30);
                ofPopStyle();
                ofPopMatrix();
                
            }
            
        }else{
            
            ofPoint pos1 = mMotionExtractor.getPositionAt(0);
            ofPoint pos2 = mMotionExtractor.getPositionAt(1);
            ofLine(pos1,pos2);
        }
        
        ramEndCamera();
    }
    
    void drawActor(const ramActor &actor){
        
    }
    
    void onEnabled(){
      
         mThread.start();
        
    }
    
    void onDisabled(){
         mThread.stop();
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
        if(name == "speed"){
            mThread.setSpeed(mSpeed);
        }
        
        if(name == "range"){
            mThread.setRange(mRange);
        }
    }
    
private:
    
    float mSpeed = 1.0;
    float mRange = 0.0;
    float mRad = 100.0;
    float mLimit = 0.25;
    float mVelScale = 1.0;
    float mSpeedDefault = 1.0;
    
    dpServoPendulumThread mThread;
    
    ramMotionExtractor mMotionExtractor;
    
    KezSlide mAngle;
    
    ofxOscSender mSender;
    
    float mScale = 1.0;
    bool isRotMode = false;
};

#endif
