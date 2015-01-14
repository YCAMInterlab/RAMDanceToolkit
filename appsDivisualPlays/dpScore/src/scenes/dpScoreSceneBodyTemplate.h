//
//  dpScoreSceneBodyTemplate.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyTemplate__
#define __dpScore__dpScoreSceneBodyTemplate__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

struct BodyTemplateNode final : public ofxMot::Node {
    BodyTemplateNode();
    virtual ~BodyTemplateNode() = default;
    BodyTemplateNode& operator = (const BodyTemplateNode& rhs);
};

class SceneBodyTemplate final : public SceneBodyBase<BodyTemplateNode> {
public:
    explicit SceneBodyTemplate() = default;
    virtual ~SceneBodyTemplate() = default;
    
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
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyTemplate__) */
