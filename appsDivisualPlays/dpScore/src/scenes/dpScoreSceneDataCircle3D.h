//
//  dpScoreSceneDataCircle3D.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#ifndef __dpScore__dpScoreSceneDataCircle3D__
#define __dpScore__dpScoreSceneDataCircle3D__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataCircle3D final : public SceneBase {
public:
    explicit SceneDataCircle3D() = default;
    virtual ~SceneDataCircle3D() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    struct Circle {
        Circle();
        void update(const ofVec2f& v);
        void draw();
        
        float mRadius{270.f};
        const int kResolution = 200;
        deque<ofVec2f> data;
    };
    vector<ofPtr<Circle> > mCircles;
    const int kNumCircles = 1;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataCircle3D__) */
