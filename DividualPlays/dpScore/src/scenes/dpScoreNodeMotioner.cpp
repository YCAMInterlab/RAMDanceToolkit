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

}

NodeMotioner::~NodeMotioner()
{

}

void NodeMotioner::customDraw()
{
	ScopedStyle s;
	setStyle();

	auto skl = ofxMot::SkeletonBase<ofxMot::Node>::create();
	ofxMotioner::drawSkeleton(skl);
}

DP_SCORE_NAMESPACE_END
