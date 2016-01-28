//
//  dpScoreNodeMasterHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeMasterHakoniwa.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

NodeMasterHakoniwa::NodeMasterHakoniwa()
{
	title = "Master Hakoniwa";
	titleJP = "マスター箱庭";
	descriptionJP = "「マスター箱庭」\n各ダンサーの運動量で溢れる色水の量が変化\n水槽の中の状態を解析して利用する箱庭を選択する";

	setGlobalPosition(ofVec3f(450.f, 0.f, 400.f - Desk::getDimension()));
	addAimingOffset(ofVec3f(Desk::getDimension() * 0.5f, Desk::getHeight() + 50.f, Desk::getDimension() * 0.5f + 30.f));
	getCamera().setFov(45.f);
	getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getDimension() * 0.5f + 50.f, 200.f - 75.f, 550.f);

	const float w {60.f};
	const float h {73.f};
	const float x {(mDesk.getDimension() - w) * 0.5f};
	mFront.setup(ofVec3f(x, mDesk.getHeight(), 50.f), w, h, 10.f);
	mRear.setup(ofVec3f(x, mDesk.getHeight(), 50.f), w, h, 30.f);

}

NodeMasterHakoniwa::~NodeMasterHakoniwa()
{

}

void NodeMasterHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	mDesk.draw();

	mFront.draw();
	mRear.draw();
}

DP_SCORE_NAMESPACE_END