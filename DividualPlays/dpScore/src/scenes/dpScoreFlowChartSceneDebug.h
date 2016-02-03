//
//  dpScoreFlowChartSceneDebug.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartSceneDebug_h
#define dpScoreFlowChartSceneDebug_h

#include "dpScoreFlowChartBaseScene.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartSceneDebug: public FlowChartBaseScene {
	void reset(class SceneFlowChart* owner) override;
	void update(class SceneFlowChart* owner) override;
	void drawText(class SceneFlowChart* owner) override;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartSceneDebug_h */
