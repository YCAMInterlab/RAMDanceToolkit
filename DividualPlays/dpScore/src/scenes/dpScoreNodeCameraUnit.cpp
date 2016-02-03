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
#include "dpScoreNodeComputer.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

NodeCameraUnit::NodeCameraUnit()
{
    title = "Hakoniwa Analysis";
    titleJP = "映像解析";
    descriptionJP = "「映像解析」\n箱庭の内部の状態をコンピュータで解析して\nRAMシステムに伝えます";
    
    const float diff {NodeStage::kDepth};
    setGlobalPosition(-NodeStage::kWidth * 0.5f - Deck::getWidth() - 320.f - getWidth() * 0.7f,
                      0.f,
                      NodeStage::kDepth * 0.5f - 300.f + getDepth() * 0.9f);
    addAimingOffset(ofVec3f(7.f, getHeight() * 0.83f, -35.f));
    
    getCamera().setFov(30.f);
    auto p = getGlobalPosition();
    p.x -= getWidth() * 2.0f;
    p.y += getHeight() * 1.6f;
    p.z -= getDepth() * 0.55f;
    getCamera().setPosition(p);
    getCamera().setOrientation(ofVec3f(-30.f, 180.f + 45.f + 45.f, 0.f));
    
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
    ScopedRotateY rY(45.f);
    
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