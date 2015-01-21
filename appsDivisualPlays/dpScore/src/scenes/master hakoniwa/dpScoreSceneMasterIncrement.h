//
//  dpScoreSceneMasterIncrement.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#ifndef __dpScore__dpScoreSceneMasterIncrement__
#define __dpScore__dpScoreSceneMasterIncrement__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"
#include "ofxOsc.h"
#include "dpScoreMasterHakoniwa.h"
#include "dpScoreAnalyzeMean.h"

DP_SCORE_NAMESPACE_BEGIN

struct MasterIncrementNode final : public ofxMot::Node {
    MasterIncrementNode();
    virtual ~MasterIncrementNode() = default;
    MasterIncrementNode& operator = (const MasterIncrementNode& rhs);
    
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

class SceneMasterIncrement final : public SceneBodyBase<MasterIncrementNode> {
public:
    explicit SceneMasterIncrement() = default;
    virtual ~SceneMasterIncrement() = default;
    
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
    vector<float> mAdditions;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneMasterIncrement__) */
