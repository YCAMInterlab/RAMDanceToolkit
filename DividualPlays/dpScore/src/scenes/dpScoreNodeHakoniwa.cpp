//
//  dpScoreNodeHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeHakoniwa.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

NodeHakoniwa::NodeHakoniwa()
{
	title = "Hakoniwa x 7";
	titleJP = "箱庭（７種類）";
	descriptionJP = "「箱庭（７種類）」\n仮想環境の情報を箱庭に伝えることによって\n箱庭の内部の状態が変化します";

	setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 157.f,
	                  0.f,
	                  NodeStage::kDepth * 0.5f - NodeHakoniwa::getDepth() - 10.f);
	addAimingOffset(ofVec3f(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight(), NodeHakoniwa::getDepth() * 0.5f));

	getCamera().setFov(80.f);
	auto p = getGlobalPosition();
	p.x += Desk::getWidth() + 150.f;
	p.y += 250.f;
	p.z += Desk::getDepth() * kNumY * 0.5f;
	getCamera().setPosition(p);
	//getCamera().setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() * 0.5f - 50.f, 250.f, 550.f);
	getCamera().setOrientation(ofVec3f(-30.f, 90.f, 0.f));

	//mLights.clear();
	//for (auto i : rep(kNumX * kNumY)) {
	//    mLights.push_back(Box::create(ofVec3f(Desk::getDimension() - 5.f, Desk::getHeight(), Desk::getDimension() - 9.f), 5.f, 75.f, 9.f));
	//}

	mDesks.clear();
	for (auto i : rep(kNumX * kNumY)) {
		mDesks.push_back(Desk());
	}
}

NodeHakoniwa::~NodeHakoniwa()
{

}

void NodeHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
	const float t {::fmodf(getElapsedTime() * 2.f, kNumX * kNumY)};
	for (auto i : rep(kNumX)) {
		for (auto j : rep(kNumY)) {
			//ScopedTranslate t(i * Desk::getWidth(), 0.f, j * (Desk::getDepth() + kGapZ));
			ScopedTranslate t(i * Desk::getWidth(), 0.f, j * Desk::getDepth());
			mDesks.at(i * kNumY + j).draw();
			//mLights.at(i * kNumY + j).draw();
		}
	}
	const float yOffset {0.f};
	if (mFocus) {
		const int time = (int)getElapsedTime() * 0.25f;
		mX = time % kNumX;
		mY = time % (kNumX * kNumY) / kNumX;

		ofSetColor(color::kMain);
		ofDisableDepthTest();
		ScopedTranslate t(mX * Desk::getWidth(), 0.f, mY * Desk::getDepth());
		mDesks.at(mX * kNumY + mY).draw();
		//mLights.at(mX * kNumY + mY).draw();

		mOffsets.front().set(Desk::getWidth() * mX + Desk::getWidth() * 0.5f,
		                     Desk::getHeight() + yOffset,
		                     Desk::getDepth() * mY + Desk::getDepth() * 0.5f);
	}
	else {
		mOffsets.front().set(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight() + yOffset, NodeHakoniwa::getDepth() * 0.5f);
	}
}

DP_SCORE_NAMESPACE_END

