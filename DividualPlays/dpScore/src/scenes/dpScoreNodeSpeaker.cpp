//
//  dpScoreNodeSpeaker.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeSpeaker.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeSpeaker::NodeSpeaker()
{
    title = "Speakers";
    titleJP = "音響";
    descriptionJP = "音響";
    
	addAimingOffset(getRight() + ofVec3f(getWidth() * 0.5f - 32.f, getHeight() * 0.7f, 45.f));
    
    getCamera().setFov(55.f);
    getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getDimension() * 0.5f + 50.f - 40.f, -100.f, 600.f);
    getCamera().setOrientation(ofVec3f(0.f, -45.f, 0.f));
    
    mBottom.setup(ofVec3f::zero(), getWidth(), getHeight(), getDepth());
    mTop.setup(ofVec3f::zero(), 35.f, 62.f, 44.f);
}

NodeSpeaker::~NodeSpeaker()
{

}

void NodeSpeaker::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	const float tx {(getWidth() - 35.f) * 0.5f};
	auto drawSpeakers = [&]() {
				    mBottom.draw();
				    ofTranslate(tx, getHeight(), getDepth() - 44.f);
				    ofRotateX(-5.f);
				    mTop.draw();
			    };
	{
		ScopedMatrix m;
		ofTranslate(getLeft());
		ofRotateY(45.f);
		drawSpeakers();
	}
	{
		ScopedMatrix m;
		ofTranslate(getRight());
		ofRotateY(-45.f);
		drawSpeakers();
	}
}

ofVec3f NodeSpeaker::getLeft()
{
	return ofVec3f(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 54.f - 50.f, 0.f, NodeStage::kDepth * 0.5f + 50.f);
}

ofVec3f NodeSpeaker::getRight()
{
	return ofVec3f(NodeStage::kWidth * 0.5f + NodeHakoniwa::getWidth() + 50.f - 54.f, 0.f, NodeStage::kDepth * 0.5f + 50.f - 69.f);
}

DP_SCORE_NAMESPACE_END