//
//  dpScoreSceneBodyScan.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyScan__
#define __dpScore__dpScoreSceneBodyScan__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

struct BodyScanNode final : public ofxMot::Node {
    typedef ofPtr<BodyScanNode> Ptr;
    BodyScanNode();
    virtual ~BodyScanNode();
    BodyScanNode& operator = (const BodyScanNode& rhs) { return *this = rhs; }
    
    void update();
    void customDraw() override;
    
    vector<float> spd;
    vector<ofVec3f> axis;
    vector<ofVec3f> vertices;
    vector<ofVec3f> initialVertices;
    
    const int kNumVertices{230};
    ofVbo vbo;
    float scale{90.f};
};

class SceneBodyScan final : public SceneBodyBase<BodyScanNode> {
public:
    explicit SceneBodyScan() = default;
    virtual ~SceneBodyScan() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void updateSkeleton(SkeletonPtr skl);
    
private:
    ofEasyCam mCam;
    
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyScan__) */
