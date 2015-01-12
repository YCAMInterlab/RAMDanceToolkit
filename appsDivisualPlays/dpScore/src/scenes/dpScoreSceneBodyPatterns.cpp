//
//  dpScoreSceneBodyPatterns.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneBodyPatterns.h"

DP_SCORE_NAMESPACE_BEGIN


void SceneBodyPatterns::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mCam.setDistance(200);
    mCam.disableMouseInput();
}

void SceneBodyPatterns::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyPatterns::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyPatterns::onUpdateSkeleton);
    mCam.enableMouseInput();
}

void SceneBodyPatterns::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyPatterns::onUpdateSkeleton);
    mCam.disableMouseInput();
}

void SceneBodyPatterns::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner) {
    }
}

static void _drawSkeleton(ofxMot::SkeletonPtr skl)
{
    ofPushMatrix();
    ofPushStyle();
    
    auto& joints = skl->getJoints();
    
    ofNoFill();
    
    ofSetColor(ofColor::white);
    
    for (size_t i=0; i<joints.size(); i++) {
        ofSetLineWidth(1.0f);
        auto& n = skl->getJoint(i);
        n.transformGL();
        const float s = 5.f;
        ofDrawBox(ofVec3f::zero(), s);
        n.restoreTransformGL();
        
        if (!n.getParent()) continue;
    
        ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}

void SceneBodyPatterns::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(getName(), 12.f, 16.f);
    
    const float stepX = alignf(kW/nX);
    const float stepY = alignf((kH-20.f)/nY);
    for (int j=0; j<nY; j++) {
        for (int i=0; i<nX; i++) {
            ofRectangle viewport(i*stepX, j*stepY+20.f, stepX, stepY);
            mCam.begin(viewport);
            const int idx = i + j * nX;
            if (idx<mSkeletons.size()) {
                ofPushMatrix();
                ofTranslate(0.f, -70.f);
                _drawSkeleton(mSkeletons.at(idx));
                ofPopMatrix();
            }
            mCam.end();
            ofNoFill();
            ofSetLineWidth(1.f);
            ofSetColor(ofColor::white);
            ofRect(viewport);
        }
    }
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyPatterns::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        auto copy = ofxMot::Skeleton::copy(skl);
        mSkeletons.push_back(copy);
        
        while (mSkeletons.size() > kNumSkeletons) {
            mSkeletons.pop_front();
        }
    }
}

DP_SCORE_NAMESPACE_END