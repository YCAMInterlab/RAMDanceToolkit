//
//  dpScoreSceneDataSphere.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#ifndef __dpScore__dpScoreSceneDataSphere__
#define __dpScore__dpScoreSceneDataSphere__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataSphere final : public SceneBase {
public:
    explicit SceneDataSphere() = default;
    virtual ~SceneDataSphere() = default;
    
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
        
        float mRadius{320.f};
        const int kResolution = 200;
        deque<ofVec2f> data;
    };
    vector<ofPtr<Circle> > mCircles;
    const int kNumCircles = 3;
    
    ofEasyCam mCam;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataSphere__) */
