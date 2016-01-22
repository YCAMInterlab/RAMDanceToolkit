//
//  dpScoreSceneBodyPatterns.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyPatterns__
#define __dpScore__dpScoreSceneBodyPatterns__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN



class SceneBodyPatterns final : public SceneBase {
public:
    explicit SceneBodyPatterns() = default;
    virtual ~SceneBodyPatterns() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    void drawSkeleton(ofxMot::SkeletonPtr skl, int idx);

    struct Range {
        Range(int b, int e) : begin(b), end(e) {}
        int begin{0};
        int end{0};
    };
    vector<Range> mRange;
    
    string mSkeletonName;
    deque<ofxMot::SkeletonPtr> mSkeletons;
    const int nX{8};
    const int nY{5};
    const int kNumSkeletons = nX * nY;
    
    ofEasyCam mCam;
    unsigned long mFrame{0};
    int mFocusNode{0};
    const int kSkip{15};
    int mCurrentRange{0};
    const float mScale{2.0f};
    ofRectangle mViewport;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyPatterns__) */
