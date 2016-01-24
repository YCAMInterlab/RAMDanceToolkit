//
//  dpScoreNodeComputer.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeComputer.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeComputer::NodeComputer()
{
    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - Deck::getWidth() - 50.f - 30.f, 0.f, -450.f);
    addAimingOffset(NodeComputer::getMacBookPosition(3) + ofVec3f(MacBook::getDepth() * 0.5f,
                                                                  MacBook::getDepth() * 0.5f,
                                                                  -MacBook::getWidth() * 0.5f));
}

NodeComputer::~NodeComputer()
{

}

void NodeComputer::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	for (auto i : rep(kNumDecks)) {
		ScopedTranslate t(0.f, 0.f, mDeck.getDepth() * i);
		mDeck.draw();
	}

	const float step {(Deck::getDepth() * kNumDecks) / (float)kNumMacBooks};
	for (auto i : rep(kNumMacBooks)) {
		ScopedTranslate trans(mDeck.getWidth() * 0.2f, mDeck.getHeight(), step * 0.5f + i * step + mMBP.getWidth() * 0.5f);
		ScopedRotateY ry(90.f);
		mMBP.angle = t * kMaxMacBookAngle;
		mMBP.draw();
	}
}

ofVec3f NodeComputer::getMacBookPosition(int i)
{
	const float step {(Deck::getDepth() * kNumDecks) / (float)kNumMacBooks};
    return ofVec3f(Deck::getWidth() * 0.2f, Deck::getHeight(), step * 0.5f + i * step + MacBook::getWidth() * 0.5f);
}

DP_SCORE_NAMESPACE_END