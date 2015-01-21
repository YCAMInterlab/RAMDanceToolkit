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

    ramMotionExtractor	motionExtractor;
    ofxOscSender mOscSender;
    
    bool mAllTurn, mGear1, mGear2, mGear3, mDatahow, mManual;
    bool mAllGearReverse, mGear1Reverse, mGear2Reverse, mGear3Reverse;
    int mScale, mAllGearSpeed, mGear1Speed, mGear2Speed, mGear3Speed;
    int mGear1Count, mGear2Count, mGear3Count, mTotalcount;
    int mMax1speed, mMax2speed, mMax3speed;
    int mMin1speed, mMin2speed, mMin3speed;
    
    int mMotionExtCnt;

};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
