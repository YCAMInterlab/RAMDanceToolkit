//
//  dpScoreFlowChartSceneMove.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneMove.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreBaseNode.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneMove::update(SceneFlowChart* owner)
{
    super::update(owner);

	float t {ofClamp(mLineTime, 0.f, mMoveSpan) / mMoveSpan};
	t = easeInOutCubic(t);
	for (auto& p : owner->getNodes()) {
		if (p.first == owner->getNextNodeName()) {
			p.second->t = t;
		}
		else if (p.first == owner->getCurrentNodeName()) {
			p.second->t = 1.f - t;
		}
		else {
			p.second->t = 0.f;
		}
	}
	auto currCam = owner->getCurrentNode()->getCamera();
	auto nextCam = owner->getNextNode()->getCamera();
	auto p = currCam.getGlobalPosition().interpolated(nextCam.getGlobalPosition(), t);
	auto q0 = currCam.getGlobalOrientation();
	auto q1 = nextCam.getGlobalOrientation();
	ofQuaternion q;
	q.slerp(t, q0, q1);
	mCamera->setFov(currCam.getFov() * (1.f - t) + nextCam.getFov() * t);
	mCamera->setGlobalPosition(p);
	mCamera->setGlobalOrientation(q);
}

void FlowChartSceneMove::drawText(SceneFlowChart* owner)
{
	auto next = owner->getNextNode();
	if (next->t >= 1.f) {
		const string str {next->title};
		{
			ScopedTranslate t(25.f, 170.f);
			ofSetColor(ofColor::white);
			owner->getFont().drawString(str, 0.f, 0.f);
		}
		const string strJP {next->titleJP};
		{
			ScopedTranslate t((owner->kWidth - owner->getFontJP().stringWidth(strJP)) * 0.5f,
			                  owner->kHeight - 40.f);
			ofSetColor(ofColor::black, 180);
			ofRect(owner->getFontJP().getStringBoundingBox(strJP, 0.f, 0.f));
			ofSetColor(ofColor::white);
			owner->getFontJP().drawStringAsShapes(strJP, 0.f, 0.f);
		}
	}
}

DP_SCORE_NAMESPACE_END