//
//  dpScoreSceneBodyBox.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyBox__
#define __dpScore__dpScoreSceneBodyBox__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyBox;

struct BodyBoxNode final : public ofxMot::Node {
    BodyBoxNode();
    virtual ~BodyBoxNode();
    BodyBoxNode& operator = (const BodyBoxNode& rhs);
    
    void update();
    
    vector<ofVec3f> screenCoords;
    vector<vector<ofVec3f> > randomPoints;
    
    SceneBodyBox* owner{nullptr};
    
    const int kMaxDivision = 10;
    const int kNumRandomPoints = 2;
};

class SceneBodyBox final : public SceneBodyBase<BodyBoxNode> {
    friend struct BodyBoxNode;
public:
    explicit SceneBodyBox() = default;
    virtual ~SceneBodyBox() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void setupSkeleton(SkeletonPtr skl) override;
    void updateSkeleton(SkeletonPtr skl) override;
    void exitSkeleton(SkeletonPtr skl) override;
    void guiEvent(ofxUIEventArgs &e) override;
    
private:
    void updateLights();
    
    ofPtr<ofFbo> mGridFbo;
    bool mFboReady{false};
    ofEasyCam mCam;
    float mDiv{20.f};
    
    const int kNumLights = 1;
    vector<ofPtr<ofLight> > mLights;
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyBox__) */
