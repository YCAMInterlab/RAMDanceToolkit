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

class AnalyzeMean {
public:
    void update(const ofVec4f& mean)
    {
        mMean = mean;
        
        mMeanAddtion.x += ::fabsf((mMean.x - mMeanPrev.x)) * 0.01;
        mMeanAddtion.y += ::fabsf((mMean.y - mMeanPrev.y)) * 0.01;
        mMeanAddtion.z += ::fabsf((mMean.z - mMeanPrev.z)) * 0.01;
        mMeanAddtion.w += ::fabsf((mMean.w - mMeanPrev.w)) * 0.01;
        
        mMeanPrev = mMean;
        
        if (mMeanAddtion.x >= mMeanLimit) {
            mMeanAddtion.x = 0.f;
            const int which = ofRandom(2);
            const int scene = ofClamp(which * 3 + 0, 0, kNumScenes);
            getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
            getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
            mPrevScene = scene;
        }
        if (mMeanAddtion.y >= mMeanLimit) {
            mMeanAddtion.y = 0.f;
            const int which = ofRandom(2);
            const int scene = ofClamp(which * 3 + 1, 0, kNumScenes);
            getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
            getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
            mPrevScene = scene;
        }
        if (mMeanAddtion.z >= mMeanLimit) {
            mMeanAddtion.z = 0.f;
            const int which = ofRandom(2);
            const int scene = ofClamp(which * 3 + 2, 0, kNumScenes);
            getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
            getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
            mPrevScene = scene;
        }
    }
    
    ofVec4f mMean;
    ofVec4f mMeanPrev;
    ofVec4f mMeanAddtion;
    float mMeanLimit{5};
    int mPrevScene{0};
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
    AnalyzeMean mMean;
    
    float mLimit{1000.f};
    unsigned long mFrameNum{0};
    unsigned long mPrevOfFrame{0};
    vector<float> mAdditions;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneMasterIncrement__) */
