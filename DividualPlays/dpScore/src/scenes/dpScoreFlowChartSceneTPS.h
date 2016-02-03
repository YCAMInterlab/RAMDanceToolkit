//
//  dpScoreFlowChartSceneTPS.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartSceneTPS_h
#define dpScoreFlowChartSceneTPS_h

#include "dpScoreFlowChartBaseScene.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartSceneTPS : public FlowChartBaseScene {
    void reset(class SceneFlowChart* owner) override;
    void update(class SceneFlowChart* owner) override;
    void drawText(class SceneFlowChart* owner) override;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartSceneTPS_h */
