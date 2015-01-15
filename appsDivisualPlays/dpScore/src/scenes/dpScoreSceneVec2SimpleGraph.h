//
//  dpScoreSceneVec2SimpleGraph.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneVec2SimpleGraph__
#define __dpScore__SceneVec2SimpleGraph__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneVec2SimpleGraph final : public SceneBase {
public:
    explicit SceneVec2SimpleGraph() = default;
    virtual ~SceneVec2SimpleGraph() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mBuffer;
    float mSensorScale{0.8f};
    ofVec2f mVec{0.f};
    int mStep{4};
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2SimpleGraph__) */
