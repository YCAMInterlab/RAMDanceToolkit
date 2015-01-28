//
//  dpScoreSceneMasterDecrement.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/15.
//
//

#ifndef dpScore_dpScoreSceneMasterDecrement_h
#define dpScore_dpScoreSceneMasterDecrement_h

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"
#include "ofxOsc.h"
#include "dpScoreMasterHakoniwa.h"
#include "dpScoreAnalyzeMean.h"

DP_SCORE_NAMESPACE_BEGIN

struct MasterDecrementNode final : public ofxMot::Node {
    MasterDecrementNode();
    virtual ~MasterDecrementNode() = default;
    MasterDecrementNode& operator = (const MasterDecrementNode& rhs);
    
    void update()
    {
        const auto pos = getGlobalPosition();
        if (!vecInited) {
            prevPos = pos;
            vecInited = true;
        }
        const auto d = pos.distance(prevPos);
        totalDistance += d * 0.1f;
        prevPos = pos;
    }
    
    ofVec3f prevPos;
    float totalDistance{0.f};
    bool vecInited{false};
};

class SceneMasterDecrement final : public SceneBodyBase<MasterDecrementNode> {
public:
    explicit SceneMasterDecrement() = default;
    virtual ~SceneMasterDecrement() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void setupSkeleton(SkeletonPtr skl) override;
    void updateSkeleton(SkeletonPtr skl) override;
    void exitSkeleton(SkeletonPtr skl) override;
    
private:
    ofEasyCam mCam;
    
    float mLimit{2000.f};
    float mDoSomething{20.f};
    float mTotal{0.f};
    vector<float> mAdditions;
};

DP_SCORE_NAMESPACE_END


#endif
