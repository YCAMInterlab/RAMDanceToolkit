//
//  dpScoreNodeDisplay.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeDisplay.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

NodeDisplay::NodeDisplay()
{

}

NodeDisplay::~NodeDisplay()
{

}

void NodeDisplay::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	{
		ScopedMatrix m;
		ofTranslate(-NodeStage::kWidth * 0.5f, 0.f, -NodeStage::kDepth * 0.5f - 10.f);
		ofRotateX(-30.f);
		drawBox(ofVec3f::zero(), 64.f, 39.f, 3.f);
	}
	{
		ScopedMatrix m;
		ofTranslate(NodeStage::kWidth * 0.5f - 64.f, 0.f, -NodeStage::kDepth * 0.5f - 10.f);
		ofRotateX(-30.f);
		drawBox(ofVec3f::zero(), 64.f, 39.f, 3.f);
	}
	{
		ScopedMatrix m;
		ofTranslate(-NodeStage::kWidth * 0.5f - 61.f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
		ofRotateY(-30.f);
		ofLine(ofVec3f(52.f - 20.f, 61.f, 1.5f), ofVec3f(52.f - 20.f, 161.f, 1.5f));
		ofLine(ofVec3f(52.f + 20.f, 61.f, 1.5f), ofVec3f(52.f + 20.f, 161.f, 1.5f));
		drawBox(ofVec3f::zero(), 104.f, 61.f, 3.f);
	}
	{
		ScopedMatrix m;
		ofTranslate(NodeStage::kWidth * 0.5f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
		ofRotateY(30.f);
		ofLine(ofVec3f(52.f - 20.f, 61.f, 1.5f), ofVec3f(52.f - 20.f, 161.f, 1.5f));
		ofLine(ofVec3f(52.f + 20.f, 61.f, 1.5f), ofVec3f(52.f + 20.f, 161.f, 1.5f));
		drawBox(ofVec3f::zero(), 104.f, 61.f, 3.f);
	}
}

DP_SCORE_NAMESPACE_END