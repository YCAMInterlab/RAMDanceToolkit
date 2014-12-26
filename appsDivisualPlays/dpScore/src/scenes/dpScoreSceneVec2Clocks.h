//
//  dpScoreSceneVec2Clocks.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneVec2Clocks__
#define __dpScore__SceneVec2Clocks__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneVec2Clocks final : public SceneBase {
public:
    SceneVec2Clocks() {}
    virtual ~SceneVec2Clocks() {}
    
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

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2Clocks__) */
