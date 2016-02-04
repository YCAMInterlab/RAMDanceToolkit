//
//  dpScoreNodeMotioner.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeMotioner.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

NodeMotioner::NodeMotioner()
{
	title = "Motion Captures";
	titleJP = "モーションキャプチャ";
	descriptionJP = "身体の動きをセンサーで情報化し";

	addAimingOffset(ofVec3f::zero());

	getCamera().setFov(70.f);
	getCamera().setPosition(0.f, 400.f, 400.f);
	getCamera().setOrientation(ofVec3f(-30.f, 0.f, 0.f));
}

NodeMotioner::~NodeMotioner()
{

}

void NodeMotioner::customDraw()
{

}

DP_SCORE_NAMESPACE_END
