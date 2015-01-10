//
//  dpScoreSceneBodyFlow.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneBodyFlow.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBodyFlow::Points::Points()
{
    for (int i=0; i<30; i++) {
        p0.push_back(randVec3f() * 30.f);
    }
    for (int i=0; i<20; i++) {
        p1.push_back(randVec3f() * 20.f);
    }
    for (int i=0; i<10; i++) {
        p2.push_back(randVec3f() * 10.f);
    }
    v0.setVertexData(&p0.at(0), p0.size(), GL_DYNAMIC_DRAW);
    v1.setVertexData(&p1.at(0), p1.size(), GL_DYNAMIC_DRAW);
    v2.setVertexData(&p2.at(0), p2.size(), GL_DYNAMIC_DRAW);
}

void SceneBodyFlow::Points::update()
{
    for (auto& p : p0) {
        p += randVec3f() * 0.3f;
    }
    for (auto& p : p1) {
        p += randVec3f() * 0.2f;
    }
    for (auto& p : p2) {
        p += randVec3f() * 0.1f;
    }
    v0.updateVertexData(&p0.at(0), p0.size());
    v1.updateVertexData(&p1.at(0), p1.size());
    v2.updateVertexData(&p2.at(0), p2.size());
}

void SceneBodyFlow::Points::draw()
{
    glPointSize(3.f);
    v0.draw(GL_POINTS, 0, p0.size());
    glPointSize(2.f);
    v1.draw(GL_POINTS, 0, p1.size());
    glPointSize(1.f);
    v2.draw(GL_POINTS, 0, p2.size());
}

void SceneBodyFlow::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mCam.setDistance(200);
    
    mPoints.assign(kNumSkeletons, ofPtr<Points>());
    for (auto& p : mPoints) {
        p = ofPtr<Points>(new Points());
    }
}

void SceneBodyFlow::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyFlow::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyFlow::onUpdateSkeleton);
}

void SceneBodyFlow::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyFlow::onUpdateSkeleton);
}

void SceneBodyFlow::update(ofxEventMessage& m)
{
    ofSetWindowTitle(getName() + ": " + ofToString(ofGetFrameRate(), 2));
    
    if (m.getAddress() == kAddrMotioner) {
        for (auto p : mPoints) p->update();
    }
}

void SceneBodyFlow::drawSkeleton(int indx)
{
    ofPushMatrix();
    ofPushStyle();
    
    auto& joints = mSkeletons.at(indx)->getJoints();
    
    ofNoFill();
    
    ofSetColor(255, 64);
    
    for (size_t i=0; i<joints.size(); i++) {
        ofSetLineWidth(1.0f);
        auto& n = mSkeletons.at(indx)->getJoint(i);
        n.transformGL();
        mPoints.at(i)->draw();
        n.restoreTransformGL();
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}

void SceneBodyFlow::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(getName(), 12.f, 16.f);
    
    mCam.begin();
    ofTranslate(100.f, 0.f);
    ofRotateX(13.f);
    ofRotateY(47.f);
    ofPushMatrix();
    ofTranslate(0.f, -60.f, 100.f);
    
    for (int i=0; i<mSkeletons.size(); i++) {
        ofTranslate(0.f, 0.f, -10.f);
        drawSkeleton(i);
    }
    ofPopMatrix();
    mCam.end();
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyFlow::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        auto copy = ofxMot::copySkeleton(skl);
        mSkeletons.push_back(copy);
        
        while (mSkeletons.size() > kNumSkeletons) {
            mSkeletons.pop_front();
        }
    }
}

DP_SCORE_NAMESPACE_END
