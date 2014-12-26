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
    SceneVec2Plotter() {}
    virtual ~SceneVec2Plotter() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mCircleBuffer;
    vector<ofVec3f> mCircleVertices;
    vector<ofFloatColor> mCircleColors;
    ofVbo mCircleVbo;
    
    float mSensorScale;
    ofVec2f mVec;
};


DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2Plotter__) */
