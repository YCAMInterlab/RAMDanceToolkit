//
//  dpScoreSceneVec2Plotter.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneVec2Plotter__
#define __dpScore__SceneVec2Plotter__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneVec2Plotter final : public SceneBase {
public:
    explicit SceneVec2Plotter() = default;
    virtual ~SceneVec2Plotter() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    static const int _circleNum{2000};
    const float kStepZ{10.0f};
    
    deque<ofVec2f> mCircleBuffer;
    vector<ofVec3f> mCircleVertices;
    vector<ofFloatColor> mCircleColors;
    ofVbo mCircleVbo;
    ofEasyCam mCam;
    
    float mSensorScale{1.0f};
    ofVec2f mVec{0.f};
    float mEnterTime{0.f};
};


DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2Plotter__) */
