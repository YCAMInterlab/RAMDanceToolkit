//
//  dpScoreVec2Clocks.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreVec2Clocks__
#define __dpScore__dpScoreVec2Clocks__

#include "dpScoreBase.h"

class dpScoreVec2Clocks final : public dpScoreBase {
public:
    dpScoreVec2Clocks() {}
    virtual ~dpScoreVec2Clocks() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mClockBuffer;
    float mSensorScale;
    ofVec2f mVec;
    
};

#endif /* defined(__dpScore__dpScoreVec2Clocks__) */
