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
//#include "ParallelLinkManager.h"
//konno--------------
#include "ofxKsmrStepManager.h"
#include "axisConverter.h"
//konno--------------
class HakoniwaGearMove : public ramBaseScene {
public:
    
    HakoniwaGearMove();
    virtual ~HakoniwaGearMove(){};
    
    void setupControlPanel();
    void update();
    void draw();
    void onPanelChanged(ofxUIEventArgs& e);
    
    void drawActor(const ramActor &actor);
    
    inline
    string getName() const { return "HakoniwaGearMove"; }
    
    //konno----------
    ofxKsmrStepManager KsmrStep;
    bool ocsTurn;
    bool turn;
    bool kimeuchiTurn;
    bool x;
    bool y;
    bool z;
    int axisNum;
    int nodeNumber;
    int nodeNumberBefore;
    int count;
    float justTrurnSpeed;
    float beforeVal;
    float kimeuchiVal;
    ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
    bool mNodeVisibility[ramActor::NUM_JOINTS];
    axisConverter   converter;
    //konno----------
    
private:
    ofxOscSender mOscSender;
    ramActor mActor;
//    static const int kNumValves = 3;
//    
//    
//    //============================================
//    ParallelLinkManager mLinkManager;
//    
//    bool CalibratePose;
//    bool ManualPose;
//    ofVec3f mManualPosition;
};


#endif /* defined(__RAMDanceToolkit__HakoniwaParallelLink_Base__) */
