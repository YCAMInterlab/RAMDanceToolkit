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
    
};

DP_SCORE_NAMESPACE_END


#endif /* defined(__dpScore__dpScoreSceneDataDisplacement__) */
