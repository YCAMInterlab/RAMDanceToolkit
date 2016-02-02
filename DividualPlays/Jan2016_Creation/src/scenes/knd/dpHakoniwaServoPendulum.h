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
        ramGetGUI().addSlider("outputRange", 0.0, 90, &mRange);
        
        ramGetGUI().addSlider("currentRange", 0.0, 90, &mCurrentRange);
        ramGetGUI().addSlider("threshMin", 0.0, 90.0, &mThreshMin);
        ramGetGUI().addSlider("threshMax", 0.0, RANGE_MAX, &mThreshMax);
        ramGetGUI().addSlider("length", 0.01, 400.0, &mLength);
        ramGetGUI().addSlider("currentLength", 0.01, 400.0, &mCurrentLength);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(350,32));
      //  mMotionExtractor.load("motionExt_dpHServoPendulum.xml");
        
        int rnd = ofRandom(0,2);
        
        //if(rnd == 0)mThreshMax = 38;
        //else mThreshMax = RANGE_MAX;
        
        //mThreshMax = RANGE_MAX;
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaServoPendulum::onPanelChanged);
    }
    void setup(){
        
        mThread.setup();

    }
    
    void setRange(float range){
        
        mRange = range;
        mRange = fmaxf(0, mRange);
        mRange = fminf(90,mRange);
        
        mThread.setRange(mRange);
        
    }
    
    void update(){
    
        ofPoint pos1 = mMotionExtractor.getPositionAt(0);
        ofPoint pos2 = mMotionExtractor.getPositionAt(1);
    
        float val = ofMap((pos1 - pos2).length(),0,mLength,0,90);
        
        mCurrentLength = (pos1 - pos2).length();
        
        mCurrentRange = val;
        
        if(val < mThreshMin)val = 0.0;
        
        if(val > mThreshMax)val = 0.0;
        
        setRange(val);
        
        mThread.setSpeed(mSpeed);
        
        mMotionExtractor.update();

    }
    
    void draw(){
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        
        ramBeginCamera();
        
        {
            
            ofPoint pos1 = mMotionExtractor.getPositionAt(0);
            ofPoint pos2 = mMotionExtractor.getPositionAt(1);
            ofLine(pos1,pos2);
            
            /*ofPushMatrix();
            ofRotateZ(mThread.getAngle());
            ofLine(0,0,0,100);
            ofCircle(0,100,40);
            ofPopMatrix();*/
        }
        
        mMotionExtractor.draw();
        
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
    
    static const int RANGE_MAX = 90;
    
    float mLength = 200.0;
    float mThreshMin = 24.5;
    float mThreshMax = 38;//RANGE_MAX;
    
    float mSpeed = 1.0;
    float mRange = 0.0;
    float mCurrentRange = 0.0;
    float mCurrentLength = 0.0;
    
    dpServoPendulumThread mThread;
    
    ramMotionExtractor mMotionExtractor;
    
};

#endif
