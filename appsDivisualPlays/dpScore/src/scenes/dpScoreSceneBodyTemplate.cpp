//
//  dpScoreSceneBodyTemplate.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#include "dpScoreSceneBodyTemplate.h"

DP_SCORE_NAMESPACE_BEGIN

BodyTemplateNode::BodyTemplateNode()
{
}

BodyTemplateNode& BodyTemplateNode::operator = (const BodyTemplateNode& rhs)
{
    return *this = rhs;
}

#pragma mark ___________________________________________________________________
void SceneBodyTemplate::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
}

void SceneBodyTemplate::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyTemplate::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
}

void SceneBodyTemplate::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneBodyTemplate::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        
    }
}

void SceneBodyTemplate::draw()
{
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        
        mCam.begin();
        ofPushMatrix();
        ofNoFill();
        const int n = getNumSkeletons();
        const float step = kW/n;
        ofTranslate(-kW*0.5f + step * 0.5f + step * i , -300.f, 0.f);
        
        ofPopMatrix();
        mCam.end();
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyTemplate::setupSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyTemplate::updateSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyTemplate::exitSkeleton(SkeletonPtr skl)
{
    
}

DP_SCORE_NAMESPACE_END