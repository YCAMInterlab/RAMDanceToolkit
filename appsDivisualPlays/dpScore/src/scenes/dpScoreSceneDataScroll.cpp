//
//  dpScoreSceneDataScroll.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneDataScroll.h"

DP_SCORE_NAMESPACE_BEGIN


void SceneDataScroll::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
}

void SceneDataScroll::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataScroll::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneDataScroll::onUpdateSkeleton);
}

void SceneDataScroll::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneDataScroll::onUpdateSkeleton);
}

void SceneDataScroll::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner) {

    }
}

void SceneDataScroll::drawSkeleton(int index)
{
    ofPushMatrix();
    ofPushStyle();
    
    auto s = mSkeletons.at(index);
    auto& joints = s->getJoints();
    
    float y = 30.f + 10 * index;
    float x = 24.f;
    const float xStep = 96.f;
    
    auto shift = [&x](float f) {
        float ret = 0.f;
        if (f<0.f) ret -= 8.f;
        if (::fabsf(f)>=100.f) ret -= 16.f;
        else if (::fabsf(f)>=10.f) ret -= 8.f;
        return ret;
    };
    
    for (size_t i=5; i<joints.size(); i++) {
        if (x > kW-xStep) break;
        auto& n = s->getJoint(i);
        ofVec3f v = n.getGlobalPosition();
        ofQuaternion q = n.getGlobalOrientation();
        
        string s = ofToString(v.x);
        ofDrawBitmapString(s, x + shift(v.x), y);
        //x += (s.size()+1) * 8.f;
        x += xStep;
        
        s = ofToString(v.y);
        ofDrawBitmapString(s, x + shift(v.y), y);
        //x += (s.size()+1) * 8.f;
        x += xStep;
        
        s = ofToString(v.z);
        ofDrawBitmapString(s, x + shift(v.z), y);
        //x += (s.size()+1) * 8.f;
        x += xStep;
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}

void SceneDataScroll::draw()
{
    ofPushMatrix();
    for (int i=0; i<mSkeletons.size(); i++) {
        drawSkeleton(i);
    }
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneDataScroll::onUpdateSkeleton(ofxMotioner::EventArgs &e)
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
