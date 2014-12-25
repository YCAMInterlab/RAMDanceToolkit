//
//  dpScoreVec2SimpleGraph.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreVec2SimpleGraph__
#define __dpScore__dpScoreVec2SimpleGraph__

#include "dpScoreBase.h"

class dpScoreVec2SimpleGraph final : public dpScoreBase {
public:
    dpScoreVec2SimpleGraph() {}
    virtual ~dpScoreVec2SimpleGraph() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mBuffer;
    float mSensorScale;
    ofVec2f mVec;
    int mStep;
    
};

#endif /* defined(__dpScore__dpScoreVec2SimpleGraph__) */
