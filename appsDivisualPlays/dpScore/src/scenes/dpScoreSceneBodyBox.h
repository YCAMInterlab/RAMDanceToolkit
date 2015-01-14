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

struct BodyBoxNode final : public ofxMot::Node {
    BodyBoxNode();
    virtual ~BodyBoxNode() = default;
    BodyBoxNode& operator = (const BodyBoxNode& rhs);
    
    void update();

    vector<ofVec2f> screenCoords;
};

class SceneBodyBox final : public SceneBodyBase<BodyBoxNode> {
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
    
private:
    ofEasyCam mCam;
    float mDiv{10.f};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyBox__) */
