//
//  dpScoreNodeDancer.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/23/16.
//
//

#include "dpScoreNodeDancer.h"

DP_SCORE_NAMESPACE_BEGIN

NodeDancer::NodeDancer()
{
	title = "Dancers";
	titleJP = "ダンサー";
	descriptionJP = "ダンサー\nディスプレイの映像や音響、照明を利用する";

	addAimingOffset(ofVec3f::zero());

	getCamera().setFov(50.f);
	getCamera().setPosition(0.f, 400.f, 800.f);
	getCamera().setOrientation(ofVec3f(-20.f, 0.f, 0.f));
}

NodeDancer::~NodeDancer()
{
}

void NodeDancer::customDraw()
{
}

DP_SCORE_NAMESPACE_END