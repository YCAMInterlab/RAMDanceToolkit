//
//  dpScoreFlowChartSceneCirculation.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneCirculation.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeLight.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

static const float kCamSpeed {0.025f};

void FlowChartSceneCirculation::reset(SceneFlowChart* owner)
{
	super::reset(owner);
	mElapsedTime = HALF_PI * (1.f / kCamSpeed);
	mNodeCenter.resetTransform();
	mCamera->clearParent();
	mCamera->resetTransform();
	mCamera->setParent(mNodeCenter);
	mCamera->setGlobalPosition(0.f, 200.f, 1200.f);
}

void FlowChartSceneCirculation::update(SceneFlowChart* owner)
{
	super::update(owner);

	const float r {::cosf(mElapsedTime * kCamSpeed) * 90.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	owner->getNode<NodeLight>()->t = 0.f;
}

void FlowChartSceneCirculation::drawText(SceneFlowChart* owner)
{
	auto curr = owner->getCurrentNode();
	auto next = owner->getNextNode();

	const string strFrom {curr->title};
	const string strTo {"\n" + next->title};
	{
		ScopedTranslate t(25.f, 112.f);
		ofSetColor(color::kMain);
		owner->getFontSmall().drawString("from\nto", 0.f, 0.f);
		ofSetColor(ofColor::white);
		owner->getFontSmall().drawString(strFrom, owner->getFontSmall().stringWidth("fromx"), 0.f);
		owner->getFontSmall().drawString(strTo, owner->getFontSmall().stringWidth("fromx"), 0.f);
	}
	const string strJP {curr->titleJP + "　→　" + next->titleJP};
	{
		ScopedTranslate t((owner->kWidth - owner->getFontJP().stringWidth(strJP)) * 0.5f, owner->kHeight - 40.f);
		ofSetColor(ofColor::black, 180);
		ofRect(owner->getFontJP().getStringBoundingBox(strJP, 0.f, 0.f));
		ofSetColor(ofColor::white);
		owner->getFontJP().drawStringAsShapes(strJP, 0.f, 0.f);
	}
}

DP_SCORE_NAMESPACE_END