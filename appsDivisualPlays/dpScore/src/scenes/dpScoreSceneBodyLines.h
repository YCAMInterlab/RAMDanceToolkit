//
//  dpScoreSceneBodyLines.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/11/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyLines__
#define __dpScore__dpScoreSceneBodyLines__

#include "dpScoreSceneBodyBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

struct BodyLinesNode final : public ofxMot::Node {
    BodyLinesNode();
    virtual ~BodyLinesNode() = default;
    BodyLinesNode& operator = (const BodyLinesNode& rhs) { return *this = rhs; }
    
    void setupPoints();
    void setupLines();
    
    void updatePoints();
    void updateLines(bool focus);
    void draw(bool focus);
    void drawHUD(bool focus);
    
    vector<float> spd;
    vector<ofVec3f> axis;
    vector<ofVec3f> vertices;
    vector<ofVec3f> initialVertices;
    
    const int kNumVertices{30};
    ofVbo vbo;
    
    vector<ofVec3f> verticesLines;
    vector<ofFloatColor> verticesColorsW;
    vector<ofFloatColor> verticesColorsR;
    ofVbo vboLines;
    float scale{0.f};
    bool pFocus;
    
    ofVec3f windowPos;
};

class SceneBodyLines final : public SceneBodyBase<BodyLinesNode> {
public:
    explicit SceneBodyLines() = default;
    virtual ~SceneBodyLines() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void setupSkeleton(SkeletonPtr skl) override;
    void updateSkeleton(SkeletonPtr skl) override;
    
private:
    bool isFocus(int nodeId);
    
    ofEasyCam mCam;
    const int kFocusLoop{240};
    int mFocusNode{0};
    unsigned long mFrameNum{0};
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneBodyLines__) */
