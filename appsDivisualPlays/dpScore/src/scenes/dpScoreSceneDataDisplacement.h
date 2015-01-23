//
//  dpScoreSceneDataDisplacement.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#ifndef __dpScore__dpScoreSceneDataDisplacement__
#define __dpScore__dpScoreSceneDataDisplacement__

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneDataDisplacement final : public SceneBase {
public:
    explicit SceneDataDisplacement() = default;
    virtual ~SceneDataDisplacement() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
private:
    void updateShader(float t, float sx, float sy);
    ofShader mShader;
    ofEasyCam mCam;
    ofVboMesh mSphereMesh;
    const float kRadius{500.f};
    const int kResolution{30};
    float mFreq{30.f};
    float mAmp{20.f};
    ofVec3f mSpeed{0.09f, 0.12f};
    float mEnterTime{0.f};
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataDisplacement__) */
