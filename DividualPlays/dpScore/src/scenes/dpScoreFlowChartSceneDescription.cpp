//
//  dpScoreFlowChartSceneDescription.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneDescription.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeLight.h"
#include "dpScoreNodeSpeaker.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeCameraUnit.h"
#include "dpScoreNodeComputer.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneDescription::reset(SceneFlowChart* owner)
{
	super::reset(owner);
	mCamera->setGlobalPosition(-NodeStage::kWidth * 0.5f + 100.f, 600.f, 800.f);
	mCamera->setOrientation(ofVec3f(-30.f, 0.f, 0.f));
}

void FlowChartSceneDescription::update(SceneFlowChart* owner)
{
	super::update(owner);
	owner->getNode<NodeLight>()->t = 0.f;
	owner->getNode<NodeSpeaker>()->t = 0.f;
}

void FlowChartSceneDescription::drawText(SceneFlowChart* owner)
{
	auto next = owner->getNextNode();

	const string str {next->title};
	{
		ScopedTranslate t(25.f, 170.f);
		ofSetColor(ofColor::white);
		owner->getFont().drawString(str, 0.f, 0.f);
	}
	string strJP {next->descriptionJP};
	if (owner->getCurrentNodeName() == getClassName<NodeCameraUnit>() and
	    owner->getNextNodeName() == getClassName<NodeComputer>()) {
		strJP = owner->getNode<NodeComputer>()->descriptionJPFromHakoniwa;
	}

	auto lines = ofSplitString(strJP, "\n");
	float t {mLineTime / mMoveSpan};
	t = ofClamp(t, 0.f, 1.f - FLT_EPSILON);
	const decltype(lines.size())index = lines.size() * t;
	auto line = lines.at(index);

	if (mTotalTime - mElapsedTime <= mMoveSpan * 4.f) {
		line = "このやりとりの繰り返しでダンスがつくられます";
	}
	{
		ScopedTranslate t((owner->kWidth - owner->getFontJP().stringWidth(line)) * 0.5f,
		                  owner->kHeight - 40.f);
		ofSetColor(ofColor::black, 180);
		ofRect(owner->getFontJP().getStringBoundingBox(line, 0.f, 0.f));
		ofSetColor(ofColor::white);
		owner->getFontJP().drawStringAsShapes(line, 0.f, 0.f);
	}
}

DP_SCORE_NAMESPACE_END