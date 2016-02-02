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
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeMasterHakoniwa::NodeMasterHakoniwa()
{
	title = "Master Hakoniwa";
	titleJP = "マスター箱庭";
    descriptionJP = "「マスター箱庭」\n本公演では特殊な役割の箱庭がひとつあります\nこの箱庭がうごきだすと\nダンサーの運動量に応じて色水が落ち\nその混ざり具合で次の仮想環境が選ばれます";

    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 157.f,
                      Desk::getHeight(),
                      NodeStage::kDepth * 0.5f - NodeHakoniwa::getDepth() - 10.f + NodeHakoniwa::getDepth() - 30.f);
	addAimingOffset(ofVec3f(Desk::getWidth() * 0.5f, Desk::getHeight() + 50.f, Desk::getDepth() * 0.5f + 30.f));
	getCamera().setFov(45.f);
	getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getWidth() * 0.5f + 50.f, 200.f - 75.f, 550.f);

	const float w {60.f};
	const float h {73.f};
    mFront.setup(ofVec3f::zero(), w, h, 10.f);
	mRear.setup(ofVec3f::zero(), w, h, 30.f);

}

NodeMasterHakoniwa::~NodeMasterHakoniwa()
{

}

void NodeMasterHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	//mDesk.draw();
    ofSetColor(color::kMain);

	mFront.draw();
	mRear.draw();
}

DP_SCORE_NAMESPACE_END