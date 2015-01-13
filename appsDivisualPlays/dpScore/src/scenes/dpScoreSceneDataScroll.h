//
//  dpScoreSceneDataScroll.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#ifndef __dpScore__dpScoreSceneDataScroll__
#define __dpScore__dpScoreSceneDataScroll__


#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataScroll final : public SceneBase {
public:
    explicit SceneDataScroll() = default;
    virtual ~SceneDataScroll() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void drawSkeleton(int indx);
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    string mSkeletonName{""};
    deque<ofxMot::SkeletonPtr> mSkeletons;
    const int kNumSkeletons{(kH-20) / 10};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneDataScroll__) */
