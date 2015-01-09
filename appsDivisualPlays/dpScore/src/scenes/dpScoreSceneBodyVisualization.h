//
//  dpScoreSceneBodyVisualization.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#ifndef __dpScore__dpScoreSceneBodyVisualization__
#define __dpScore__dpScoreSceneBodyVisualization__

#include "dpScoreSceneBase.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class SceneBodyVisualization final : public SceneBase {
public:
    explicit SceneBodyVisualization() = default;
    virtual ~SceneBodyVisualization() = default;
    
    void initialize() override;
    void shutDown() override;
    
    void enter() override;
    void exit() override;
    
    void update(ofxEventMessage& m) override;
    void draw() override;
    
    void onUpdateSkeleton(ofxMotioner::EventArgs &e);
    void onDrawSkeleton(ofxMotioner::EventArgs &e);
    
private:
    ofEasyCam mCam;
    
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreSceneBodyVisualization__) */
