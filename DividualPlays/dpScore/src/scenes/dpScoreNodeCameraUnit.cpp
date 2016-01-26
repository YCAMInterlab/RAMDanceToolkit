//
//  dpScoreNodeCameraUnit.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeCameraUnit.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeCameraUnit::NodeCameraUnit()
{
    title = "Hakoniwa Analysis";
    titleJP = "映像解析";
    descriptionJP = "箱庭の状態を映像解析";
    
    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 250.f, 0.f, 100.f);
    addAimingOffset(ofVec3f(getWidth() * 0.5f, getHeight() * 0.82f, -20.f));
    
    getCamera().setFov(35.f);
    getCamera().setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 300.f, 0.f, -50.f);
    getCamera().setOrientation(ofVec3f(-30.f, 180.f + 30.f, 0.f));
}

NodeCameraUnit::~NodeCameraUnit()
{

}

void NodeCameraUnit::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
	drawBox(ofVec3f::zero(), getWidth(), getHeight(), getDepth());

	const float x {(getWidth() - 44.f) * 0.5f};
	ScopedMatrix m;
	ofTranslate(x, 83.f, 0.f);
	drawBox(ofVec3f::zero(), 44.f, 5.f, -35.f);

	ofTranslate((44.f - mMBP.getWidth()) * 0.5f, 5.f, -35.f);
	mMBP.angle = t * kMaxMacBookAngle;
	mMBP.draw();
}

DP_SCORE_NAMESPACE_END