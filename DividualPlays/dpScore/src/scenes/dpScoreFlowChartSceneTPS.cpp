//
//  dpScoreFlowChartSceneTPS.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneTPS.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneTPS::reset()
{
    super::reset();
    mCamera->setParent(mNodeCenter);
    mCamera->setGlobalPosition(0.f, 0.f, 200.f);
}

void FlowChartSceneTPS::update(SceneFlowChart* owner)
{
    super::update(owner);
}

void FlowChartSceneTPS::drawText(SceneFlowChart* owner)
{
    const string str {"Dancer's viewpoint"};
    {
        ScopedTranslate t(25.f, 170.f);
        ofSetColor(ofColor::white);
        owner->getFont().drawString(str, 0.f, 0.f);
    }
    const string strJP {"ダンサー視点"};
    {
        ScopedTranslate t((owner->kWidth - owner->getFontJP().stringWidth(strJP)) * 0.5f,
                          owner->kHeight - 40.f);
        ofSetColor(ofColor::black, 180);
        ofRect(owner->getFontJP().getStringBoundingBox(strJP, 0.f, 0.f));
        ofSetColor(ofColor::white);
        owner->getFontJP().drawStringAsShapes(strJP, 0.f, 0.f);
    }
}

DP_SCORE_NAMESPACE_END