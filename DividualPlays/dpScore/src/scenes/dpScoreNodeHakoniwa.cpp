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
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

const int NodeHakoniwa::kNumDesks = 3;

NodeHakoniwa::NodeHakoniwa()
{
	title = "Hakoniwa x 7";
	titleJP = "箱庭（７種類）";
	descriptionJP = "RAMシステムはその情報を\n光や音や映像へと変換します";

	setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 157.f,
	                  0.f,
	                  NodeStage::kDepth * 0.5f - NodeHakoniwa::getDepth() - 10.f);
	addAimingOffset(ofVec3f(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight(), NodeHakoniwa::getDepth() * 0.5f));

	getCamera().setFov(80.f);
	auto p = getGlobalPosition();
	p.x += Desk::getWidth() + 150.f;
	p.y += 250.f;
	p.z += Desk::getDepth() * kNumDesks * 0.5f;
	getCamera().setPosition(p);
	getCamera().setOrientation(ofVec3f(-30.f, 90.f, 0.f));

	mDesks.clear();
	for (auto i : rep(kNumDesks)) {
		mDesks.push_back(Desk());
	}

	mHakoniwas.clear();
	mHakoniwas.assign(NUM_HAKONIWA_TYPES, ofPtr<BaseHakoniwa>());
	mHakoniwas.at(HAKO_TORNADO) = makeShared<HakoTornado>();
	mHakoniwas.at(HAKO_WORM) = makeShared<HakoWorm>();
	mHakoniwas.at(HAKO_STRUGGLE) = makeShared<HakoStruggle>();
	mHakoniwas.at(HAKO_SERVO_PENDULUM) = makeShared<HakoServoPendulum>();
	mHakoniwas.at(HAKO_GEAR) = makeShared<HakoGear>();
	mHakoniwas.at(HAKO_MAG_PENDULUM) = makeShared<HakoMagPendulum>();
	mHakoniwas.at(HAKO_SAND_STORM) = makeShared<HakoSandStorm>();

	mHakoOrigin.setOrientation(ofVec3f(0.f, -90.f, 0.f));
	for (auto p : mHakoniwas) {
		p->setParent(mHakoOrigin);
	}

	ofVec3f v(10.f, Desk::getHeight());
	mHakoniwas.at(HAKO_TORNADO)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_TORNADO)->getDepth() * 0.5f) + v);
	v += ofVec3f(10.f + mHakoniwas.at(HAKO_TORNADO)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_WORM)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_WORM)->getDepth() * 0.5f) + v);

	v += ofVec3f(10.f + mHakoniwas.at(HAKO_WORM)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_STRUGGLE)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_STRUGGLE)->getDepth() * 0.5f) + v);

	v += ofVec3f(10.f + mHakoniwas.at(HAKO_STRUGGLE)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_SERVO_PENDULUM)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_SERVO_PENDULUM)->getDepth() * 0.5f) + v);

	v += ofVec3f(20.f + mHakoniwas.at(HAKO_SERVO_PENDULUM)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_GEAR)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_GEAR)->getDepth() * 0.5f) + v);

	v += ofVec3f(60.f + mHakoniwas.at(HAKO_GEAR)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_MAG_PENDULUM)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_MAG_PENDULUM)->getDepth() * 0.5f) + v);

	v += ofVec3f(60.f + mHakoniwas.at(HAKO_MAG_PENDULUM)->getWidth(), 0.f);
	mHakoniwas.at(HAKO_SAND_STORM)->setPosition(ofVec3f(0.f, 0.f, -Desk::getWidth() * 0.5f - mHakoniwas.at(HAKO_SAND_STORM)->getDepth() * 0.5f) + v);
}

NodeHakoniwa::~NodeHakoniwa()
{

}

void NodeHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	for (auto i : rep(kNumDesks)) {
		ScopedTranslate t(0.f, 0.f, i * Desk::getDepth());
		mDesks.at(i).draw();
	}
	for (auto i : rep(mHakoniwas.size())) {
		if (mFocus && i == mCurrHakoniwa) {
			continue;
		}
		mHakoniwas.at(i)->draw();
	}

	if (mFocus) {
		const int time = (int)getElapsedTime() * 0.25f;
		mCurrHakoniwa = time % mHakoniwas.size();
		ofSetColor(color::kMain);
		auto hako = mHakoniwas.at(mCurrHakoniwa);
		hako->draw();
		ofVec3f v = hako->getGlobalPosition();
		v.x -= hako->getDepth() * 0.5f;
		v.y += hako->getHeight() * 0.5f;
		v.z += hako->getWidth() * 0.5f;
		mOffsets.front() = v;
	}
	else {
		mOffsets.front().set(NodeHakoniwa::getWidth() * 0.5f, Desk::getHeight(), NodeHakoniwa::getDepth() * 0.5f);
	}
}

DP_SCORE_NAMESPACE_END

