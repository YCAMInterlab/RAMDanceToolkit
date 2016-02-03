//
//  dpScoreFlowChartRDTK.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreFlowChartRDTK_h
#define dpScoreFlowChartRDTK_h

#include "dpScoreCommon.h"
#include "dpScoreSceneFlowChart.h"

DP_SCORE_NAMESPACE_BEGIN

struct FlowChartRDTK {
    void draw(SceneFlowChart::SkeletonVec& skeletons);
private:
    void drawFloor();
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartRDTK_h */
