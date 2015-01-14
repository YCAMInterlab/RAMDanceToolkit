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
#include "axisConverter.h"
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
    string getName() const { return "HakoniwaGearMove"; }
    
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
    axisConverter   converter;
    
    bool mturn, mgear1, mgear2, mgear3, mDatahow, mManual;
    bool mturnReverse, mgear1Reverse, mgear2Reverse, mgear3Reverse;
    int mScale, TurnSpeed, TurnGear1Speed, TurnGear2Speed, TurnGear3Speed;
    int CountGear1Speed, CountGear2Speed, CountGear3Speed;
    int TurnGear1Count, TurnGear2Count, TurnGear3Count;
    int Max1speed, Max2speed, Max3speed;
    int Min1speed, Min2speed, Min3speed;
    
    int count, motionExtCnt;
    float totalSpeed;
    int avgSpeed[3];

};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
