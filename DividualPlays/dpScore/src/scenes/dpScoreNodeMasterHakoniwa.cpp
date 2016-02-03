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
	                  Desk::getHeight() + 0.5f,
	                  NodeStage::kDepth * 0.5f - NodeHakoniwa::getDepth() - 10.f + NodeHakoniwa::getDepth() - 30.f);
	addAimingOffset(ofVec3f(Desk::getWidth() * 0.5f, Desk::getHeight() + 50.f, Desk::getDepth() * 0.5f + 30.f));
	getCamera().setFov(45.f);
	getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getWidth() * 0.5f + 50.f, 200.f - 75.f, 550.f);

	const float w {getWidth()};
	const float h {getHeight()};
	const float th {getHeightTank()};
	mFrontTop.setup(ofVec3f(0.f, th, 0.f), w, h - th, getDepthFront());
	mFrontBottom.setup(ofVec3f::zero(), w, th, getDepthFront());
	mRear.setup(ofVec3f::zero(), w, h, getDepthRear());

	for (auto j : rep(kNumZ)) {
		for (auto i : rep(kNumX)) {
			mFunnel.push_back(Funnel::create(ofVec3f::zero(), 5.75f, 0.3f, 9.f, h - th - 4.f));
			mMotors.push_back(Box::create(ofVec3f::zero(), 3.f, 3.f, 4.f));
			mValves.push_back(Cylinder::create(ofVec3f::zero(), 0.75f, 3.f, 8));
		}
	}

	mWater.setup(getWidth(), getDepthFront(), 30, 12);
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

	mRear.draw();
	{
		ScopedTranslate t(0.f, 0.f, getDepthRear());
		mFrontTop.draw();
		mFrontBottom.draw();
		const float w {getWidth()};
		const float d {getDepthFront()};
		const float sx {w / kNumX};
		const float sz {d / kNumZ};
		for (auto j : rep(kNumZ)) {
			for (auto i : rep(kNumX)) {
				const auto n = i + j * kNumX;
				{
					ScopedTranslate trans(sx * 0.5f + i * sx, getHeight(), sz * 0.5f + j * sz);
					mFunnel.at(n).draw();
				}
				{
					ScopedTranslate trans(sx * 0.5f + i * sx, getHeightTank() + 1.5f, sz * 0.5f + j * sz);
					mValves.at(n).draw();
				}
				{
					ScopedTranslate trans(sx * 0.5f + i * sx - 1.5f,
					                      getHeightTank(),
					                      sz * 0.5f + j * sz - 4.f - 0.75f);
					mMotors.at(n).draw();
				}
			}
		}
		{
			ScopedTranslate trans(0.f, getHeightTank() - 10.f, 0.f);
			mWater.update();
            ofSetColor(ofColor::white);
			mWater.draw();
		}
	}
}

float NodeMasterHakoniwa::getWidth()
{
	return 60.f;
}
float NodeMasterHakoniwa::getHeight()
{
	return 73.f;
}
float NodeMasterHakoniwa::getHeightTank()
{
	return 36.f;
}
float NodeMasterHakoniwa::getDepth()
{
	return getDepthFront() + getDepthRear();
}
float NodeMasterHakoniwa::getDepthFront()
{
	return 20.f;
}
float NodeMasterHakoniwa::getDepthRear()
{
	return 10.f;
}

DP_SCORE_NAMESPACE_END