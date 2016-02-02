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
    descriptionJP = "「ダンサー」\nダンサーはディスプレイに表示されている\n映像などの情報をもとにして\n次の動きを作り出します";

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