//
//  dpScoreSceneDataCircle.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#ifndef __dpScore__dpScoreSceneDataCircle__
#define __dpScore__dpScoreSceneDataCircle__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataCircle final : public SceneBase {
public:
    explicit SceneDataCircle() = default;
    virtual ~SceneDataCircle() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    struct Circle {
        Circle();
        void update(float f);
        void draw();
        
        float mRadius{350.f};
        const int kResolution = 200;
        deque<float> data;
    };
    vector<ofPtr<Circle> > mCircles;
    const int kNumCircles{ofVec2f::DIM};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneDataCircle__) */
