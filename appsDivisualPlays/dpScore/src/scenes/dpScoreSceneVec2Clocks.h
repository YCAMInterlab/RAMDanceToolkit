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
    explicit SceneVec2Clocks() = default;
    virtual ~SceneVec2Clocks() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    static const int _clockNumX{16};
    static const int _clockNumY{9};
    
    deque<ofVec2f> mClockBuffer;
    float mSensorScale{0.3f};
    ofVec2f mVec{0.f};
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2Clocks__) */
