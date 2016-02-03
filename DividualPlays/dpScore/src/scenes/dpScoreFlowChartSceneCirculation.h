//
//  dpScoreFlowChartSceneCirculation.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartSceneCirculation_h
#define dpScoreFlowChartSceneCirculation_h

#include "dpScoreFlowChartBaseScene.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartSceneCirculation : public FlowChartBaseScene {
    void reset(class SceneFlowChart* owner) override;
    void update(class SceneFlowChart* owner) override;
    void drawText(class SceneFlowChart* owner) override;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartSceneCirculation_h */
