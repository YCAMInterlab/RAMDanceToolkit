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

DP_SCORE_NAMESPACE_BEGIN

NodeHakoniwa::NodeHakoniwa()
{

}

NodeHakoniwa::~NodeHakoniwa()
{

}

void NodeHakoniwa::customDraw()
{
    ScopedStyle s;
    setStyle();
	for (auto i : rep(kNumX)) {
		for (auto j : rep(kNumY)) {
			ScopedTranslate t(i * mDesk.getDimension(), 0.f, j * (mDesk.getDimension() + 90.f));
			mDesk.draw();
            drawBox(ofVec3f(Desk::getDimension() - 5.f, Desk::getHeight(), Desk::getDimension() - 9.f), 5.f, 75.f, 9.f);
		}
	}
}

DP_SCORE_NAMESPACE_END

