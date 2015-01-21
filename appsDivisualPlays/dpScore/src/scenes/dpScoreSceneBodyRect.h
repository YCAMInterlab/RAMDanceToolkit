//
//  dpScoreSceneBodyRect.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyRect__
#define __dpScore__dpScoreSceneBodyRect__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyRect;

struct BodyRectNode final : public ofxMot::Node {
    BodyRectNode();
    virtual ~BodyRectNode();
    BodyRectNode& operator = (const BodyRectNode& rhs);
    
    void update();

    vector<ofVec2f> screenCoords;
    vector<vector<ofVec2f> > randomPoints;
    
    SceneBodyRect* owner{nullptr};
    
    const int kMaxDivision = 30;
    const int kNumRandomPoints = 10;
};

class SceneBodyRect final : public SceneBodyBase<BodyRectNode> {
    friend struct BodyRectNode;
public:
    explicit SceneBodyRect() = default;
    virtual ~SceneBodyRect() = default;
    
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
    ofPtr<ofFbo> mGridFbo;
    bool mFboReady{false};
    ofEasyCam mCam;
    float mDiv{20.f};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyRect__) */
