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
        ramGetGUI().addSlider("speed", 1.0, 10.0, &mSpeedDefault);
        ramGetGUI().addSlider("range", 5, 90, &mRange);
        ramGetGUI().addSlider("rad", 5, 300, &mRad);
        ramGetGUI().addSlider("velScale", 0.01, 10.0, &mVelScale);
        ramGetGUI().addSlider("limit", 0.1, 1.0, &mLimit);
        
        mMotionExtractor.setupControlPanel(this);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaServoPendulum::onPanelChanged);
    }
    void setup(){
        
        mThread.setup();
    }
    
    void update(){
        
        float shake = 0.0;//mMotionExtractor.getVelocityAt(0).x * mVelScale;
        
        mSpeed = mSpeedDefault - mMotionExtractor.getVelocitySpeedAt(0) * mVelScale;
        
        mSpeed = fmaxf(mLimit,mSpeed);
        mThread.setSpeed(mSpeed);
        
        mThread.setShake(shake);
    
        float angle = mThread.getAngle();
        
        angle = ofDegToRad(angle + shake);
        mAngle.set(angle);
        
        mMotionExtractor.update();
        mAngle.update();
    }
    
    void draw(){
    
    }
    
    void drawActor(const ramActor &actor){
        ramSetViewPort(dpGetFirstScreenViewPort());
        float x = cos(mAngle.val) * mRad;
        float y = sin(mAngle.val) * mRad;
        
        ofPushMatrix();
        ofTranslate(mMotionExtractor.getPositionAt(0).x,
                    mMotionExtractor.getPositionAt(0).y,
                    mMotionExtractor.getPositionAt(0).z);
        ofDrawSphere(x, y, 10);
        ofSetLineWidth(2);
        ofLine(x,y,0,0);
        ofPopMatrix();
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
    float mRange = 20.0;
    float mRad = 100.0;
    
    float mLimit = 0.1;
    
    dpServoPendulumThread mThread;
    
    ramMotionExtractor mMotionExtractor;
    
    KezSlide mAngle;
    
    float mVelScale = 1.0;
    
    float mSpeedDefault = 4.0;
};

#endif
