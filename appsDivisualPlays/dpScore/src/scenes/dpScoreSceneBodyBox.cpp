//
//  dpScoreSceneBodyBox.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#include "dpScoreSceneBodyBox.h"

DP_SCORE_NAMESPACE_BEGIN

BodyBoxNode::BodyBoxNode()
{
    screenCoords.clear();
}

BodyBoxNode& BodyBoxNode::operator = (const BodyBoxNode& rhs)
{
    return *this = rhs;
}

void BodyBoxNode::update()
{
    screenCoords.clear();
    transformGL();
    const ofVec2f screenCoord = project(ofVec3f::zero());
    restoreTransformGL();
    
    if (getParent()) {
        const ofVec3f begin = screenCoord;
        getParent()->transformGL();
        const ofVec3f end = project(ofVec3f::zero());
        getParent()->restoreTransformGL();
        
        const int div = begin.distance(end) / 5;
        screenCoords.assign(div, ofVec3f::zero());
        
        for (int i=0; i<div; i++) {
            const float p = i / (float)(div-1);
            const ofVec3f v = begin.interpolated(end, p);
            screenCoords.at(i) = v;
        }
    }
    else {
        screenCoords.push_back(screenCoord);
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyBox::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Division", 4.f, 40.f, &mDiv);
    
    mCam.disableMouseInput();
}

void SceneBodyBox::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyBox::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
}

void SceneBodyBox::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneBodyBox::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner) {
        for (int i=0; i<getNumSkeletons(); i++) {
            auto skl = getSkeleton(i);
            mCam.begin();
            const int n = getNumSkeletons();
            const float step = kW/n;
            ofTranslate(-kW*0.5f + step * 0.5f + step * i , -300.f, 0.f);
            for (auto& n : skl->getJoints()) {
                n.setPosition(n.getPosition()*4.5f);
                n.update();
            }
            mCam.end();
        }
    }
}

void SceneBodyBox::draw()
{
    ofSetLineWidth(1.f);
    
    const int kDiv = (int)mDiv;
    
    ofNoFill();
    ofSetColor(128, 255);
    for (int i=0; i<kW/kDiv; i++) {
        for (int j=0; j<kH/kDiv; j++) {
            ofPushMatrix();
            alignedTranslate(i*kDiv, j*kDiv);
            alignedRect(0.f, 0.f, kDiv, kDiv);
            ofPopMatrix();
        }
    }
    
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        for (auto& n : skl->getJoints()) {
            auto drawRect = [&](const ofVec2f& v) {
                int x = (int)v.x;
                int y = (int)v.y;
                
                x = (x / kDiv) * kDiv;
                y = (y / kDiv) * kDiv;
                
                ofFill();
                ofSetColor(ofColor::white, 255);
                alignedRect(x+1.f, y+1.f, kDiv-2.f, kDiv-2.f);
                ofNoFill();
                //ofSetColor(ofColor::black, 255);
                ofSetColor(color::kMain, 255);
                alignedRect(x, y, kDiv, kDiv);
            };
            for_each(n.screenCoords.begin(), n.screenCoords.end(), drawRect);
        }
    }
    
    ofFill();
    ofSetColor(ofColor::black, 255);
    alignedRect(0.f, 0.f, kW, 20.f);
}

#pragma mark ___________________________________________________________________
void SceneBodyBox::setupSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyBox::updateSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyBox::exitSkeleton(SkeletonPtr skl)
{
    
}

DP_SCORE_NAMESPACE_END