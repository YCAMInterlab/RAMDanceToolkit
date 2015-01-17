//
//  SkipperWakeUpBeam.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/21.
//
//

#ifndef __RAMDanceToolkit__SkipperWakeUpBeam__
#define __RAMDanceToolkit__SkipperWakeUpBeam__

#include "ofMain.h"
#include "KezSlidePoint.h"

class ReachAtSkipperEvent : public ofEventArgs {
    
public:
    
    int idx;
    
    ReachAtSkipperEvent() {
        idx = 0;
    }
    
    static ofEvent <ReachAtSkipperEvent> events;
};

class SkipperWakeUpBeam{
public:
    void update(){
        
        if(enable){
        
        mPos.update();
        
        if((mPos - mTarget).length() < mThresh){
        
            static ReachAtSkipperEvent newEvent;
            //newEvent.idx = mTargetIdx;
            ofNotifyEvent(ReachAtSkipperEvent::events, newEvent);
            
            enable = false;
        }
        }
    };
    
    void launch(ofPoint pos,ofPoint target){
        enable = true;
        mTarget = target;
        mPos.imSet(pos);
        mPos.set(mTarget);
    }
    
    void draw(){
        if(enable){
            ofPushStyle();
            ofDrawSphere(mPos, mRad);
            ofPopStyle();
        }
    };
private:
    KezSlidePoint mPos;
    ofPoint mTarget;
    float mThresh = 2.0;
    bool enable = false;
    float mRad = 20.0;
};

#endif /* defined(__RAMDanceToolkit__SkipperWakeUpBeam__) */
