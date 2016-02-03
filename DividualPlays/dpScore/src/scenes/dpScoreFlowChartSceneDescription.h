//
//  dpScoreFlowChartSceneDescription.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartSceneDescription_h
#define dpScoreFlowChartSceneDescription_h

#include "dpScoreFlowChartBaseScene.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartSceneDescription : public FlowChartBaseScene {
    void reset() override;
    void update(class SceneFlowChart* owner) override;
    void drawText(class SceneFlowChart* owner) override;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartSceneDescription_h */
