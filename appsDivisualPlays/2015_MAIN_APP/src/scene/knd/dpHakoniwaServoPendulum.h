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
        ramGetGUI().addSlider("range", 0.0, 90, &mRange);
        ramGetGUI().addSlider("minThresh", 0.0, 20.0, &mMinThresh);
        ramGetGUI().addSlider("mixThresh", 0.0, RANGE_MAX, &mMaxThresh);
        ramGetGUI().addSlider("length", 0.01, 400.0, &mLength);
        
        mMotionExtractor.setupControlPanel(this);
        mMotionExtractor.load("motionExt_dpHServoPendulum.xml");
        
        int rnd = ofRandom(0,2);
        
        if(rnd == 0)mMaxThresh = 38;
        else mMaxThresh = RANGE_MAX;
        
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
        
        if(val < mMinThresh)val = 0.0;
        
        if(val > mMaxThresh)val = 0.0;
        
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
    
    static const int RANGE_MAX = 90;
    
    float mLength = 200.0;
    float mMinThresh = 6.0;
    float mMaxThresh = RANGE_MAX;
    
    float mSpeed = 1.0;
    float mRange = 0.0;
    
    dpServoPendulumThread mThread;
    
    ramMotionExtractor mMotionExtractor;
    
};

#endif
