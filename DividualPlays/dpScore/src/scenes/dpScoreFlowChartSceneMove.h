//
//  dpScoreFlowChartSceneMove.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartSceneMove_h
#define dpScoreFlowChartSceneMove_h

#include "dpScoreFlowChartBaseScene.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartSceneMove : public FlowChartBaseScene {
    void update(class SceneFlowChart* owner) override;
    void drawText(class SceneFlowChart* owner) override;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartSceneMove_h */
