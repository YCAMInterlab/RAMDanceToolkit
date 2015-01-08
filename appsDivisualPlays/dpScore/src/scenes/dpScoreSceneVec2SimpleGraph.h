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
    SceneVec2SimpleGraph() {}
    virtual ~SceneVec2SimpleGraph() {}
    
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

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2SimpleGraph__) */
