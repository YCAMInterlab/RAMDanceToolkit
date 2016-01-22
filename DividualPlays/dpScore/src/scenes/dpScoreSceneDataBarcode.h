//
//  dpScoreSceneDataBarcode.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#ifndef __dpScore__dpScoreSceneDataBarcode__
#define __dpScore__dpScoreSceneDataBarcode__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataBarcode final : public SceneBase {
public:
    explicit SceneDataBarcode() = default;
    virtual ~SceneDataBarcode() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mBuffer;
    const int kBarWidth{4};
    const float kHalfH{(kH-20.f) * 0.5f};
    float mThreshould{0.5f};
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneDataBarcode__) */
