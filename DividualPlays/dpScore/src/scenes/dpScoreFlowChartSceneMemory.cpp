//
//  dpScoreFlowChartSceneMemory.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneMemory.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeLight.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneMemory::reset(SceneFlowChart* owner)
{
	super::reset(owner);
	owner->sendLightingOsc("Off", 0.f);
	mNodeCenter.resetTransform();
	mCamera->clearParent();
	mCamera->resetTransform();
	mCamera->setParent(mNodeCenter);
	mCamera->setGlobalPosition(0.f, 300.f, 1000.f);
}

void FlowChartSceneMemory::update(SceneFlowChart* owner)
{
	super::update(owner);

	if (mElapsedTime >= 18.f) {
		compoundSetEnableCollapse(true);
	}

	const float r {-mElapsedTime * 4.f + 60.f};
	mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));

	owner->getNode<NodeLight>()->t = 0.f;
}

void FlowChartSceneMemory::drawText(SceneFlowChart* owner)
{
	const string str {"Dancer's memory"};
	{
		ScopedTranslate t(25.f, 170.f);
		ofSetColor(ofColor::white);
		owner->getFont().drawString(str, 0.f, 0.f);
	}
	const string strJP {"「記憶」\nこのシステムによるトレーニングで\nダンサーは感覚を記憶に留めます\nこの記憶も身体を制御する情報として\nつかわれます"};
	{
		const float t {mElapsedTime / 15.f};
		const auto lines = ofSplitString(strJP, "\n");
		if (t < 1.f) {
			const auto current = decltype(lines.size())(lines.size() * t);
			const auto line = lines.at(current);
			ScopedTranslate trans((owner->kWidth - owner->getFontJP().stringWidth(line)) * 0.5f,
			                      owner->kHeight - 40.f);
			ofSetColor(ofColor::black, 180);
			ofRect(owner->getFontJP().getStringBoundingBox(line, 0.f, 0.f));
			ofSetColor(ofColor::white);
			owner->getFontJP().drawStringAsShapes(line, 0.f, 0.f);
		}
	}
}

DP_SCORE_NAMESPACE_END