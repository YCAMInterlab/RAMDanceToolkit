//
//  dpScoreSceneVec2Grid.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__SceneVec2Grid__
#define __dpScore__SceneVec2Grid__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneVec2Grid final : public SceneBase {
public:
    SceneVec2Grid() {}
    virtual ~SceneVec2Grid() {}
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    deque<ofVec2f> mGridBuffer;
    vector<ofVec3f> mGridVertices;
    ofVbo mGridVbo;
    
    ofEasyCam mCam;
    
    float mSensorScale;
    ofVec2f mVec;
    
    int mGridStep;
    int mGridW;
    int mGridH;
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__SceneVec2Grid__) */
