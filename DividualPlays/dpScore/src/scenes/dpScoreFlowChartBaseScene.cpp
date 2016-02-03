//
//  dpScoreFlowChartBaseScene.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartBaseScene.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreBaseNode.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartBaseScene::setup(float move, float idle, float line, float total, bool easeyCam)
{
	mMoveSpan = move;
	mIdleSpan = idle;
	mLineSpan = line;
	mTotalTime = total;

	if (easeyCam) {
		auto easyCam = ofPtr<ofEasyCam>(new ofEasyCam());
		easyCam->setDistance(900.f);
		mCamera = easyCam;
	}
}

void FlowChartBaseScene::reset()
{
	mElapsedTime = 0.f;
    mLineTime = 0.f;
}

void FlowChartBaseScene::update(SceneFlowChart* owner)
{
    mElapsedTime += ofGetLastFrameTime();
    
	for (auto& p : owner->getNodes()) {
		p.second->t = 1.f;
	}
    
    mLineTime += ofGetLastFrameTime();
    const float total {mIdleSpan + mMoveSpan};
    if (mLineTime >= total) {
        mLineTime = 0.f;
        owner->incrementNode();
    }
}

DP_SCORE_NAMESPACE_END