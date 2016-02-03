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
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

NodeSpeaker::NodeSpeaker()
{
    title = "Speakers";
    titleJP = "音響";
    descriptionJP = "「音響」";
    
	//addAimingOffset(getRight() + ofVec3f(getWidth() * 0.5f - 32.f, getHeight() * 0.7f, 45.f));
    
    getCamera().setFov(55.f);
    getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getWidth() * 0.5f + 50.f - 40.f, 200.f - 100.f, 600.f);
    getCamera().setOrientation(ofVec3f(0.f, -45.f, 0.f));
    
    mSpeakers.clear();
    for (auto i : rep(kNumSpeakers)) {
        mSpeakers.push_back(Speaker::create(ofVec3f::zero()));
    }
    mPole.setup(ofVec3f::zero(), 2.f, getH(), 8);
    
}

NodeSpeaker::~NodeSpeaker()
{

}

void NodeSpeaker::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

    {
        ScopedMatrix m;
        ofTranslate(-NodeStage::kWidth * 0.5f - getX(),
                    getH(),
                    -NodeStage::kDepth * 0.5f + 120.f);
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, 0.f, Speaker::getDepth() * 0.5f);
            ofRotateY(90.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, 0.f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(0).draw();
        }
        ofTranslate(Speaker::getWidth() * 0.5f, -getH() * 0.5f, Speaker::getDepth() * 0.5f);
        mPole.draw();
    }
    {
        ScopedMatrix m;
        ofTranslate(NodeStage::kWidth * 0.5f + getX(),
                    getH(),
                    -NodeStage::kDepth * 0.5f + 120.f);
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, 0.f, Speaker::getDepth() * 0.5f);
            ofRotateY(-90.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, 0.f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(1).draw();
        }
        ofTranslate(Speaker::getWidth() * 0.5f, -getH() * 0.5f, Speaker::getDepth() * 0.5f);
        mPole.draw();
    }
    {
        ScopedMatrix m;
        ofTranslate(-NodeStage::kWidth * 0.5f - getX(),
                    getH() + 50.f,
                    NodeStage::kDepth * 0.5f - 100.f);
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, 0.f, Speaker::getDepth() * 0.5f);
            ofRotateY(60.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, 0.f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(2).draw();
        }
    }
    {
        ScopedMatrix m;
        ofTranslate(NodeStage::kWidth * 0.5f + getX(),
                    getH() + 50.f,
                    NodeStage::kDepth * 0.5f - 100.f);
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, 0.f, Speaker::getDepth() * 0.5f);
            ofRotateY(-60.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, 0.f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(3).draw();
        }
    }
    {
        ScopedMatrix m;
        ofTranslate(-NodeStage::kWidth * 0.5f - getX(),
                    getH() + 50.f,
                    NodeStage::kDepth * 0.5f - 120.f - Speaker::getWidth());
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, Speaker::getHeight() * 0.5f, Speaker::getDepth() * 0.5f);
            ofRotateY(90.f);
            ofRotateX(15.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, -Speaker::getHeight() * 0.5f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(4).draw();
        }
    }
    {
        ScopedMatrix m;
        ofTranslate(NodeStage::kWidth * 0.5f + getX(),
                    getH() + 50.f,
                    NodeStage::kDepth * 0.5f - 120.f - Speaker::getWidth());
        {
            ScopedMatrix m2;
            ofTranslate(Speaker::getWidth() * 0.5f, Speaker::getHeight() * 0.5f, Speaker::getDepth() * 0.5f);
            ofRotateY(-90.f);
            ofRotateX(15.f);
            ofTranslate(-Speaker::getWidth() * 0.5f, -Speaker::getHeight() * 0.5f, -Speaker::getDepth() * 0.5f);
            mSpeakers.at(5).draw();
        }
    }
}

float NodeSpeaker::getX()
{
    return 320.f;
}

float NodeSpeaker::getH()
{
    return Deck::getHeight() + 83.f;
}

DP_SCORE_NAMESPACE_END