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
	title = "Hakoniwa x 6";
	titleJP = "箱庭（６種類）";
	descriptionJP = "ダンサーの動きによって現在選択されている箱庭の状態が変化";

	setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 50.f, 0.f, 400.f - NodeHakoniwa::getDepth());
	addAimingOffset(ofVec3f(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight(), NodeHakoniwa::getDepth() * 0.5f));

	getCamera().setFov(60.f);
	getCamera().setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() * 0.5f - 50.f, 250.f, 550.f);
	getCamera().setOrientation(ofVec3f(-25.f, 0.f, 0.f));

	mLights.clear();
	for (auto i : rep(kNumX * kNumY)) {
		mLights.push_back(Box::create(ofVec3f(Desk::getDimension() - 5.f, Desk::getHeight(), Desk::getDimension() - 9.f), 5.f, 75.f, 9.f));
	}

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
	const float t {::fmodf(ofGetElapsedTimef() * 2.f, kNumX * kNumY)};
	for (auto i : rep(kNumX)) {
		for (auto j : rep(kNumY)) {
			ScopedTranslate t(i * Desk::getDimension(), 0.f, j * (Desk::getDimension() + kGapZ));
			mDesks.at(i * kNumY + j).draw();
			mLights.at(i * kNumY + j).draw();
		}
	}
	const float yOffset {0.f};
	if (mFocus) {
		const int time = (int)ofGetElapsedTimef() * 0.25f;
		mX = time % kNumX;
		mY = time % (kNumX * kNumY) / kNumX;

		ofSetColor(color::kMain);
		ofDisableDepthTest();
		ScopedTranslate trans(mX * Desk::getDimension(), 0.f, mY * (Desk::getDimension() + kGapZ));
		mDesks.at(mX * kNumY + mY).draw();
		mLights.at(mX * kNumY + mY).draw();

		mOffsets.front().set(Desk::getDimension() * mX + Desk::getDimension() * 0.5f,
		                     Desk::getHeight() + yOffset,
		                     (Desk::getDimension() + kGapZ) * mY + Desk::getDimension() * 0.5f);
	}
	else {
		mOffsets.front().set(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight() + yOffset, NodeHakoniwa::getDepth() * 0.5f);
	}
}

DP_SCORE_NAMESPACE_END

