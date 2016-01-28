//
//  dpScoreNodeCameraUnit.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeCameraUnit.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeCameraUnit::NodeCameraUnit()
{
    title = "Hakoniwa Analysis";
    titleJP = "映像解析";
    descriptionJP = "「映像解析」\n箱庭の内部の状態をコンピュータで映像解析";
    
    setGlobalPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 250.f, 0.f, 100.f);
    addAimingOffset(ofVec3f(getWidth() * 0.5f, getHeight() * 0.82f, -20.f));
    
    getCamera().setFov(35.f);
    getCamera().setPosition(-NodeStage::kWidth * 0.5f - NodeHakoniwa::getWidth() - 300.f, 200.f, -50.f);
    getCamera().setOrientation(ofVec3f(-30.f, 180.f + 30.f, 0.f));
    
    mBody.setup(ofVec3f::zero(), getWidth(), getHeight(), getDepth());
    mTray.setup(ofVec3f::zero(), 44.f, 5.f, -35.f);
}

NodeCameraUnit::~NodeCameraUnit()
{

}

void NodeCameraUnit::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
    mBody.draw();

	const float x {(getWidth() - 44.f) * 0.5f};
	ScopedMatrix m;
	ofTranslate(x, 83.f, 0.f);
    mTray.draw();

	ofTranslate((44.f - mMBP.getWidth()) * 0.5f, 5.f, -35.f);
	mMBP.angle = t * kMaxMacBookAngle;
    
    auto drawDisplay = [](float w, float h)
    {
        ScopedStyle s;
        ofSetColor(color::kMain);
        const float t {getElapsedTime() * 10.f};
        ofPolyline p;
        for (int i : rep((int)w)) {
            p.addVertex((float)i, ofSignedNoise(i * 0.3f - t) * h * 0.5f + h * 0.5f, 0.f);
        }
        p.draw();
    };
    mMBP.drawDisplay = drawDisplay;
	mMBP.draw();
}

DP_SCORE_NAMESPACE_END