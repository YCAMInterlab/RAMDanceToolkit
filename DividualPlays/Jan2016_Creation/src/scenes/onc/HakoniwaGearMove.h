//
//  HakoniwaGearMove.h
//  RAMDanceToolkit
//
//  Created by ycam on 2014/12/25.
//
//

//
//  HakoniwaParallelLink_Base.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/20.
//
//

#ifndef __RAMDanceToolkit__HakoniwaGearMove__
#define __RAMDanceToolkit__HakoniwaGearMove__

#include "ramMain.h"
#include "ofxKsmrStepManager.h"
#include "ramMotionExtractor.h"
#include "dpConstants.h"

class HakoniwaGearMove : public ramBaseScene {
public:
    
    void setupControlPanel();
    void setup();
    void update();
    void draw();
    void onPanelChanged(ofxUIEventArgs& e);
    
    inline
    string getName() const { return "dpHGearMove"; }
    
    ofxKsmrStepManager		stepManager;
    
private:

    virtual void onDisabled();
    void AllstepTurn(int speed, bool dir);
    void OnestepTurn(int ch, int speed, bool dir);
    void AllstepTurnStop();
    void OnestepTurnStop(int ch);
    void drawDump();

    void sumVelocitySpeed();
    void randomiseGearDirection();
    void calcGearSpeed();
    void reset();
    
    ramMotionExtractor	motionExtractor;
    ofxOscSender mOscSender;
    
    bool mAllTurn, mGear, mDatahow, mManual;
    bool mAllGearReverse, mGearReverse;
    int mSpeedScale;
    int mAllGearSpeed, mGearSpeed;
    int mVelocitySpeedSum;
    
    int mParameterChangeCount;
    int mMaxSpeed;
    int mMinSpeed;
    
    int mMotionExtCnt;
    
    int mParameterChangeThresh;

};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
