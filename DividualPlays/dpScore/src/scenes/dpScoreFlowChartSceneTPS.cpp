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
#include "dpScoreNodeMotioner.h"
#include "dpScoreNodeDancer.h"
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneTPS::reset(SceneFlowChart* owner)
{
	super::reset(owner);
    owner->sendLightingOsc("Stage", 0.f);
    mNodeCenter.resetTransform();
    mCamera->clearParent();
    mCamera->resetTransform();
    mCamera->setParent(mNodeCenter);
	mCamera->setGlobalPosition(0.f, 0.f, 200.f);
}

void FlowChartSceneTPS::update(SceneFlowChart* owner)
{
	super::update(owner);

	auto& skeletons = owner->getCopiedSkeletons();

	if (skeletons.size() == false) {
		return;
	}

	// TPS camera
	auto head = skeletons.front()->getJoint(ofxMot::JOINT_NECK);
	//head.pan(180.f);
	head.pan(-90.f);
	const auto v0 = head.getGlobalPosition();
	const auto v1 = mNodeCenter.getGlobalPosition();
	const auto q0 = head.getGlobalOrientation();
	const auto q1 = mNodeCenter.getGlobalOrientation();
	const float f {0.99f}; // tps camera smoothing
	ofQuaternion q;
	q.slerp(f, q0, q1);
	mNodeCenter.setGlobalPosition(v0.interpolated(v1, f));
	mNodeCenter.setGlobalOrientation(q);
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