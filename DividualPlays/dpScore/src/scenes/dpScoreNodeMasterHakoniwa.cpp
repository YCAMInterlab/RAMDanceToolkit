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
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeMasterHakoniwa::NodeMasterHakoniwa()
{
	title = "Master Hakoniwa";
	titleJP = "マスター箱庭";
    descriptionJP = "「マスター箱庭」\n本公演では特殊な役割の箱庭がひとつあります\nこの箱庭がうごきだすと\nダンサーの運動量に応じて色水が落ち\nその混ざり具合で次の仮想環境が選ばれます";

    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 157.f,
                      Desk::getHeight() + 0.5f,
                      NodeStage::kDepth * 0.5f - NodeHakoniwa::getDepth() - 10.f + NodeHakoniwa::getDepth() - 30.f);
	addAimingOffset(ofVec3f(Desk::getWidth() * 0.5f, Desk::getHeight() + 50.f, Desk::getDepth() * 0.5f + 30.f));
	getCamera().setFov(45.f);
	getCamera().setPosition(NodeStage::kWidth * 0.5f + Desk::getWidth() * 0.5f + 50.f, 200.f - 75.f, 550.f);

	const float w {getWidth()};
	const float h {getHeight()};
    const float th {getHeightTank()};
    mFrontTop.setup(ofVec3f(0.f, th, 0.f), w, h - th, getDepthFront());
    mFrontBottom.setup(ofVec3f::zero(), w, th, getDepthFront());
	mRear.setup(ofVec3f::zero(), w, h, getDepthRear());

    for (auto j : rep(kNumZ)) {
        for (auto i : rep(kNumX)) {
            mFunnel.push_back(Funnel::create(ofVec3f::zero(), 5.75f, 0.3f, 9.f, h - th - 4.f));
            mMotors.push_back(Box::create(ofVec3f::zero(), 3.f, 3.f, 4.f));
            mValves.push_back(Cylinder::create(ofVec3f::zero(), 0.75f, 3.f, 8));
        }
    }
}

NodeMasterHakoniwa::~NodeMasterHakoniwa()
{

}

void NodeMasterHakoniwa::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	//mDesk.draw();
    ofSetColor(color::kMain);

	mRear.draw();
    {
        ScopedTranslate t(0.f, 0.f, getDepthRear());
    	mFrontTop.draw();
        mFrontBottom.draw();
        const float w{getWidth()};
        const float d{getDepthFront()};
        const float sx{w / kNumX};
        const float sz{d / kNumZ};
        for (auto j : rep(kNumZ)) {
            for (auto i : rep(kNumX)) {
                const auto n = i + j * kNumX;
                {
                    ScopedTranslate trans(sx * 0.5f + i * sx, getHeight(), sz * 0.5f + j * sz);
                    mFunnel.at(n).draw();
                }
                {
                    ScopedTranslate trans(sx * 0.5f + i * sx, getHeightTank() + 1.5f, sz * 0.5f + j * sz);
                    mValves.at(n).draw();
                }
                {
                    ScopedTranslate trans(sx * 0.5f + i * sx - 1.5f,
                                          getHeightTank(),
                                          sz * 0.5f + j * sz - 4.f - 0.75f);
                    mMotors.at(n).draw();
                }
            }
        }
        {
            ScopedTranslate trans(0.f, getHeightTank() - 10.f, 0.f);
            const float t{ofGetElapsedTimef() * 0.01f};
            const auto nX = 30;
            const auto nZ = 12;
            for (auto j : rep(nZ)) {
                for (auto i : rep(nX)) {
                    auto displacement = [](int i, int j, float t) {
                        float y {0.f};
                        y += ::cosf(i * 0.3f + t * 0.98f) * 0.7f;
                        y += ::cosf(i * 0.3f + t * 10.3f) * 0.6f;
                        y += ::cosf(i * 0.3f + t * 97.6f) * 0.5f;
                        y += ::sinf(j * 0.3f + t * 0.98f) * 0.7f;
                        y += ::sinf(j * 0.3f + t * 10.3f) * 0.6f;
                        y += ::sinf(j * 0.3f + t * 97.6f) * 0.5f;
                        return y;
                    };
                    const float y0 {displacement(i, j, t)};
                    const float y1 {displacement(i, j + 1, t)};
                    const float y2 {displacement(i + 1, j + 1, t)};
                    const float y3 {displacement(i + 1, j, t)};
                    
                    const float x0 {getWidth() / (float) nX * (float)i};
                    const float z0 {getDepthFront() / (float) nZ * (float)j};
                    const float x1 {getWidth() / (float) nX * (float)(i + 1)};
                    const float z1 {getDepthFront() / (float) nZ * (float)(j + 1)};
                    auto v0 = ofVec3f(x0, y0, z0);
                    auto v1 = ofVec3f(x0, y1, z1);
                    auto v2 = ofVec3f(x1, y2, z1);
                    auto v3 = ofVec3f(x1, y3, z0);
                    ofLine(v0, v1);
                    ofLine(v1, v2);
                    ofLine(v2, v3);
                    ofLine(v3, v0);
                }
            }
        }
    }
}

float NodeMasterHakoniwa::getWidth()
{
    return 60.f;
}
float NodeMasterHakoniwa::getHeight()
{
    return 73.f;
}
float NodeMasterHakoniwa::getHeightTank()
{
    return 36.f;
}
float NodeMasterHakoniwa::getDepth()
{
    return getDepthFront() + getDepthRear();
}
float NodeMasterHakoniwa::getDepthFront()
{
    return 20.f;
}
float NodeMasterHakoniwa::getDepthRear()
{
    return 10.f;
}

DP_SCORE_NAMESPACE_END