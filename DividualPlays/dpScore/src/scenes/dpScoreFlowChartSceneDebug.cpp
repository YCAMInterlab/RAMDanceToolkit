//
//  dpScoreFlowChartSceneDebug.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneDebug.h"
#include "dpScoreSceneFlowChart.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneDebug::reset(SceneFlowChart* owner)
{
    owner->sendLightingOsc("Stage", 0.f);
}

void FlowChartSceneDebug::update(SceneFlowChart* owner)
{
    super::update(owner);
}

void FlowChartSceneDebug::drawText(SceneFlowChart* owner)
{
    
}

DP_SCORE_NAMESPACE_END