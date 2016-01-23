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

}

NodeComputer::~NodeComputer()
{

}

void NodeComputer::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	const int nX {3};
	for (auto i : rep(nX)) {
		ScopedTranslate t(0.f, 0.f, mDeck.getDepth() * i);
		mDeck.draw();
	}

	const int nPCs {8};
	const float step {(Deck::getDepth() * 3) / (nPCs)};
	for (auto i : rep(nPCs)) {
		ScopedTranslate t(mDeck.getWidth() * 0.2f, mDeck.getHeight(), step * 0.5f + i * step + mMBP.getWidth() * 0.5f);
		ScopedRotateY ry(90.f);
        mMBP.angle = macAngle;
		mMBP.draw();
	}

//	ScopedMatrix m;
//	ofTranslate(0.f, 83.f, 0.f);
//	drawBox(ofVec3f::zero(), 44.f, 5.f, -35.f);
//
//	ofTranslate((44.f - mMBP.getWidth()) * 0.5f, 5.f, -35.f);
//	mMBP.angle = Shared.macAngleCameraUnit;
//	mMBP.draw();
}

DP_SCORE_NAMESPACE_END