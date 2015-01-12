//
//  dpScoreSceneBodyFlow.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyFlow__
#define __dpScore__dpScoreSceneBodyFlow__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyFlow final : public SceneBase {
public:
    struct Points {
        Points();
        void update();
        void draw();
        vector<ofVec3f> p0;
        vector<ofVec3f> p1;
        vector<ofVec3f> p2;
        ofVbo v0, v1, v2;
    };
    
    explicit SceneBodyFlow() = default;
    virtual ~SceneBodyFlow() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void drawSkeleton(int indx);
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    
private:
    string mSkeletonName;
    deque<ofxMot::SkeletonPtr> mSkeletons;
    vector<ofPtr<Points> > mPoints;
    const int kNumSkeletons{200};
    
    ofEasyCam mCam;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyFlow__) */
