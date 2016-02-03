//
//  dpScoreFlowChartBaseScene.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartBaseScene.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreNodeDancer.h"
#include "dpScoreNodeMotioner.h"
#include "dpScoreObjects.h"
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartBaseScene::setup(float move, float idle, float line, float total, bool easeyCam)
{
	mMoveSpan = move;
	mIdleSpan = idle;
	mLineSpan = line;
	mTotalTime = total;

	if (easeyCam) {
		auto easyCam = ofPtr<ofEasyCam>(new ofEasyCam());
		easyCam->setDistance(900.f);
		mCamera = easyCam;
	}
}

void FlowChartBaseScene::reset(SceneFlowChart* owner)
{
	mElapsedTime = 0.f;
	mLineTime = 0.f;
	owner->getNode<NodeHakoniwa>()->setFocus(true);
	LineObj::enableAnimation = false;
}

void FlowChartBaseScene::update(SceneFlowChart* owner)
{
	mElapsedTime += ofGetLastFrameTime();

	for (auto& p : owner->getNodes()) {
		p.second->t = 1.f;
	}

	mLineTime += ofGetLastFrameTime();
	const float total {mIdleSpan + mMoveSpan};
	if (mLineTime >= total) {
		mLineTime = 0.f;
		owner->incrementNode();
	}
    
    auto& skeletons = owner->getCopiedSkeletons();
    if (skeletons.size() == false) {
        return;
    }
    
    auto motioner = owner->getNode<NodeMotioner>();
    auto dancer = owner->getNode<NodeDancer>();
    
    // update text
    if (skeletons.size() >= 2) {
        dancer->title = "Dancers";
        motioner->title = "Motion Captures";
    }
    else {
        dancer->title = "Dancer";
        motioner->title = "Motion Capture";
    }
    
    // update for line
    motioner->clearAimingOffsets();
    for (auto i : rep(skeletons.size())) {
        motioner->addAimingOffset(skeletons.at(i)->getJoint(ofxMot::JOINT_HIPS).getGlobalPosition());
    }
    dancer->clearAimingOffsets();
    for (auto i : rep(skeletons.size())) {
        dancer->addAimingOffset(skeletons.at(i)->getJoint(ofxMot::JOINT_HEAD).getGlobalPosition());
    }
}

DP_SCORE_NAMESPACE_END