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

DP_SCORE_NAMESPACE_BEGIN

NodeMasterHakoniwa::NodeMasterHakoniwa()
{
    setGlobalPosition(ofVec3f(450.f, 0.f, 400.f - Desk::getDimension()));
    addAimingOffset(ofVec3f(Desk::getDimension() * 0.5f, Desk::getHeight() + 50.f, Desk::getDimension() * 0.5f));
}

NodeMasterHakoniwa::~NodeMasterHakoniwa()
{

}

void NodeMasterHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	mDesk.draw();

	const float w {60.f};
	const float h {73.f};
	const float x {(mDesk.getDimension() - w) * 0.5f};
	drawBox(ofVec3f(x, mDesk.getHeight(), 50.f), w, h, 10.f);
	drawBox(ofVec3f(x, mDesk.getHeight(), 50.f), w, h, 30.f);
}

DP_SCORE_NAMESPACE_END