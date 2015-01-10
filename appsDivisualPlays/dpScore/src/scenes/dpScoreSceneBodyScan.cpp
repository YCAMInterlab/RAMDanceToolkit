//
//  dpScoreSceneBodyScan.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneBodyScan.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBodyScan::Node::Node()
{
    vertices.assign(kNumVertices, ofVec3f::zero());
    for (auto& v : vertices) {
        v = randVec3f() * scale * ofRandom(0.1f, 1.f);
    }
    initialVertices = vertices;
    
    vbo.setVertexData(&vertices.at(0), vertices.size(), GL_DYNAMIC_DRAW);
    
    axis.assign(kNumVertices, ofVec3f::zero());
    for (auto& a : axis) {
        a = randVec3f();
    }
    
    spd.assign(kNumVertices, 0.f);
    for (auto& s : spd) {
        s = ofRandom(50.f, 100.f);
    }
}


void SceneBodyScan::Node::update()
{
    for (int i=0; i<kNumVertices; i++) {
        auto iv = initialVertices.at(i);
        auto& v = vertices.at(i);
        v = iv.rotate(ofGetElapsedTimef() * spd.at(i), axis.at(i));
    }
    vbo.updateVertexData(&vertices.at(0), kNumVertices);
}

void SceneBodyScan::Node::draw()
{
    vbo.draw(GL_POINTS, 0, vertices.size());
}

void SceneBodyScan::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sphere Scale", 200.f, 600.f, &mScale);
    
    mNodes.assign(ofxMot::NUM_JOINTS, Node::Ptr());
    for (auto& p : mNodes) {
        p = Node::Ptr(new Node());
    }
    
    mCam.setDistance(200);
}

void SceneBodyScan::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyScan::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyScan::onUpdateSkeleton);
}

void SceneBodyScan::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyScan::onUpdateSkeleton);
}

void SceneBodyScan::update(ofxEventMessage& m)
{
    ofSetWindowTitle(getName() + ": " + ofToString(ofGetFrameRate(), 2));
    
    if (m.getAddress() == kAddrMotioner) {
        for (auto& mn : mNodes) mn->update();
    }
}

void SceneBodyScan::draw()
{
    ofPushStyle();
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    
    mCam.begin();
    ofPushMatrix();
    ofTranslate(0.f, -80.f, 0.f);
    
    ofNoFill();
    
    if (mSkeleton) {
        glPointSize(3.f);
        ofSetColor(255, 64);
        ofSetLineWidth(1.5f);
        auto& joints = mSkeleton->getJoints();
        for (int i=0; i<joints.size(); i++) {
            auto& n = joints.at(i);
            
            n.transformGL();
            for (auto& mn : mNodes) mn->draw();
            n.restoreTransformGL();
            
            if (!n.getParent()) continue;
            ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
        }
    }
    
    ofPopMatrix();
    mCam.end();
    
    const float w = kW;
    
    const float span = 3.f;
    const float t = ::fmodf(ofGetElapsedTimef(), span) / span;
    const float x = t * kW*3.f;
    ofPushMatrix();
    ofTranslate(kW*2.f-x, 0.f);
    ofFill();
    ofSetColor(ofColor::black);
    ofRect(-kW*2.f, 0.f, kW*2.f, kH);
    ofRect(w, 0.f, kW*3.f, kH);
    glBegin(GL_QUADS);
    glColor4f(0.f, 0.f, 0.f, 0.f); glVertex2f(0.f, 0.f);
    glColor4f(0.f, 0.f, 0.f, 0.f); glVertex2f(0.f, kH);
    glColor4f(0.f, 0.f, 0.f, 1.f); glVertex2f(w, kH);
    glColor4f(0.f, 0.f, 0.f, 1.f); glVertex2f(w, 0.f);
    glEnd();
    ofSetColor(ofColor::white, 128);
    ofSetLineWidth(1.f);
    ofLine(0.f, 0.f, 0.f, alignf(kH));
    
    ofPopMatrix();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(getName(), 12.f, 16.f);
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyScan::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        mSkeleton = skl;
    }
}

DP_SCORE_NAMESPACE_END