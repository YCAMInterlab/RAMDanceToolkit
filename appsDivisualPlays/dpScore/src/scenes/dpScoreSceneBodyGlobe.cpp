//
//  dpScoreSceneBodyGlobe.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#include "dpScoreSceneBodyGlobe.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBodyGlobe::Node::Node()
{
    points.clear();
    vertices.clear();
    vertices.assign(kMaxPoints, ofVec3f::zero());
    vbo.setVertexData(&vertices.at(0), vertices.size(), GL_DYNAMIC_DRAW);
}

void SceneBodyGlobe::Node::update()
{
    ofVec3f p = dir * scale;
    p = p * getOrientationQuat();
    
    points.push_back(p);
    
    while (points.size() > kMaxPoints) {
        points.pop_front();
    }
    
    for (int i=0; i<points.size(); i++) {
        vertices.at(i) = points.at(i);
    }
    
    vbo.updateVertexData(&vertices.at(0), vertices.size());
}

void SceneBodyGlobe::Node::customDraw()
{
    glBegin(GL_LINES);
    glColor4f(1.f, 1.f, 1.f, 0.6f);
    glVertex3fv(ofVec3f::zero().getPtr());
    glColor4f(1.f, 1.f, 1.f, 0.1f);
    glVertex3fv((dir * scale * 1.2f).getPtr());
    glEnd();
}

void SceneBodyGlobe::Node::drawPoints()
{
    glPointSize(3.f);
    ofSetColor(255, 40);
    vbo.draw(GL_POINTS, 0, vertices.size());
}

void SceneBodyGlobe::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sphere Scale", 200.f, 600.f, &mScale);
    mUICanvas->addSlider("Rotation Speed X", 0.f, 10.f, &mRotSpdX);
    mUICanvas->addSlider("Rotation Speed Y", 0.f, 10.f, &mRotSpdY);
    mUICanvas->addToggle("Magnify", &mMagnify);
    
    mNodes.assign(ofxMot::NUM_JOINTS, Node::Ptr());
    for (auto& p : mNodes) {
        p = Node::Ptr(new Node());
    }
    mCam.disableMouseInput();
}

void SceneBodyGlobe::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyGlobe::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyGlobe::onUpdateSkeleton);
    mCam.enableMouseInput();
}

void SceneBodyGlobe::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyGlobe::onUpdateSkeleton);
    mCam.disableMouseInput();
}

void SceneBodyGlobe::update(ofxEventMessage& m)
{
    mFrameNum++;
    
    if (m.getAddress() == kAddrMotioner) {
        if (mMagnify) {
            mScale += ofGetLastFrameTime() * 2.f;
            if (mScale >= 600.f) mScale = 100.f;
        }
        for (auto& p : mNodes) p->scale = mScale;
        
        if (mFrameNum%60==0) {
            mJointId++;
            mJointId %= ofxMot::NUM_JOINTS;
        }
    }
}

void SceneBodyGlobe::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    
    mCam.begin();
    ofPushMatrix();
    ofRotateX(ofGetElapsedTimef() * mRotSpdX);
    ofRotateY(ofGetElapsedTimef() * mRotSpdY);
    
    ofNoFill();
    ofSetLineWidth(1.5f);
    //ofxMotioner::debugDraw();
    
    int i=0;
    for (auto& p : mNodes) {
        ofEnableAlphaBlending();
        p->drawPoints();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        p->draw();
        //if (i==mJointId && mFrameNum%60 > 0 && mFrameNum%60 < 10) {
        if (mFrameNum%120 > 0 && mFrameNum%120 < 30) {
            ofSetColor(255, 128);
            //ofSetColor(color::kMain, 255);
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            ofPushMatrix();
            ofTranslate(p->dir * p->scale * 1.f);
            ofDrawBitmapString(ofxMot::getJointNameLower(mJointId), ofPoint::zero());
            ofPopMatrix();
        }
        
        i++;
    }
    ofPopMatrix();
    mCam.end();
    
    drawHeader();
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyGlobe::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        auto joints = skl->getJoints();
        
        for (int i=0; i<mNodes.size(); i++) {
            auto& node = mNodes.at(i);
            node->dir = joints.at(i).getPosition();
            node->dir.normalize();
            node->setOrientation(joints.at(i).getOrientationQuat());
            node->update();
        }
    }
}

DP_SCORE_NAMESPACE_END