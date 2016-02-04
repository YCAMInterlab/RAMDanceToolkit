//
//  dpScoreFlowChartSceneMasterHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#include "dpScoreFlowChartSceneMasterHakoniwa.h"
#include "dpScoreSceneFlowChart.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreNodeMasterHakoniwa.h"
#include "dpScoreNodeLight.h"
#include "dpScoreNodeSpeaker.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartSceneMasterHakoniwa::reset(SceneFlowChart* owner)
{
    super::reset(owner);
    owner->getNode<NodeHakoniwa>()->setFocus(false);
    owner->sendLightingOsc("Stage", 0.f);
    auto mh = owner->getNode<NodeMasterHakoniwa>();
    auto p = mh->getGlobalPosition();
    p.x += mh->getWidth() * 0.5f;
    p.y += mh->getHeight() * 0.5f + 5.f;
    p.z += mh->getDepth() * 0.5f;
    mNodeCenter.resetTransform();
    mNodeCenter.setGlobalPosition(p);
    
    mCamera->clearParent();
    mCamera->resetTransform();
    mCamera->setParent(mNodeCenter);
    mCamera->setPosition(ofVec3f(0.f, 0.f, 110.f));
}

void FlowChartSceneMasterHakoniwa::update(SceneFlowChart* owner)
{
    super::update(owner);
    
    const float r {mElapsedTime * 4.f - 15.f};
    mNodeCenter.setGlobalOrientation(ofQuaternion(r, ofVec3f(0.f, 1.f, 0.f)));
    
    for (auto& p : owner->getNodes()) {
        p.second->t = 0.f;
    }
    owner->getNode<NodeMasterHakoniwa>()->t = 1.f;
}

void FlowChartSceneMasterHakoniwa::drawText(SceneFlowChart* owner)
{
    const string str {"Master Hakoniwa"};
    {
        ScopedTranslate t(25.f, 170.f);
        ofSetColor(ofColor::white);
        owner->getFont().drawString(str, 0.f, 0.f);
    }
    const string strJP {"「マスター箱庭」\n色水の箱庭は それぞれのダンサーの\n動きの蓄積で 対応する色の水を滴下し\nこの色の混ざり方で ダンサーに提示される\n次の情報を選びます"};
    {
        const float t {mElapsedTime / 15.f};
        const auto lines = ofSplitString(strJP, "\n");
        if (t < 1.f) {
            const auto current = decltype(lines.size())(lines.size() * t);
            const auto line = lines.at(current);
            ScopedTranslate trans((owner->kWidth - owner->getFontJP().stringWidth(line)) * 0.5f,
                                  owner->kHeight - 40.f);
            ofSetColor(ofColor::black, 180);
            ofRect(owner->getFontJP().getStringBoundingBox(line, 0.f, 0.f));
            ofSetColor(ofColor::white);
            owner->getFontJP().drawStringAsShapes(line, 0.f, 0.f);
        }
    }
}

DP_SCORE_NAMESPACE_END
