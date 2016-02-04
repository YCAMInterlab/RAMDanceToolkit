//
//  dpScoreNodeAudience.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeAudience.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

NodeAudience::NodeAudience()
{
	title = "Audiences";
    titleJP = "観客";
	descriptionJP = "数名の集中力、疲労度を解析（JINS MEMEメガネ）";

	setGlobalPosition(0.f, 0.f,  NodeStage::kDepth * 0.5f + 500.f);
	getCamera().setFov(80.f);
	getCamera().setPosition(0.f, 300.f, 650.f);
	getCamera().setOrientation(ofVec3f(-30.f, 180.f, 0.f));
}

NodeAudience::~NodeAudience()
{
    
}

static void drawBox(const ofVec3f& p, float w, float h, float d)
{
	ofDrawBox(p.x + w * 0.5f, p.y + h * 0.5f, p.z + d * 0.5f, w, h, d);
}

void NodeAudience::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
	const float height {30.f};
	const float depth {mChair.getDepth() + 42.f};

	const int nX {15}, nZ {5};
	for (auto j : rep(nZ)) {
		for (auto i : rep(nX)) {
			const float w {nX * mChair.getWidth()};
			ScopedTranslate t(-w * 0.5f + (w / (float)nX) * i, j * height, j * depth);
			mChair.draw();
		}
		ScopedTranslate t(0.f, height * j, depth * j);
		drawBox(ofVec3f(-nX * mChair.getWidth() * 0.5f, 0.f, 0.f), -80.f, height, depth);
		drawBox(ofVec3f(nX * mChair.getWidth() * 0.5f, 0.f, 0.f), 80.f, height, depth);
	}

	for (auto i : rep(nX)) {
		const float w {nX * mChair.getWidth()};
		ScopedTranslate t(-w * 0.5f + (w / (float)nX) * i, 0.f, -220.f);
		mChair.draw();
	}

	for (auto j : rep(nZ)) {
		for (auto i : rep(2)) {
			const float w {nX * mChair.getWidth()};
			{
				ScopedTranslate t(-w * 0.5f - 80.f - mChair.getWidth() * (i + 1), j * height, j * depth);
				mChair.draw();
			}
			{
				ScopedTranslate t(w * 0.5f + 80.f + mChair.getWidth() * i, j * height, j * depth);
				mChair.draw();
			}
		}
	}
}


DP_SCORE_NAMESPACE_END
