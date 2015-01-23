//
//  dpScoreSceneBodyScan.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneBodyScan.h"

DP_SCORE_NAMESPACE_BEGIN

BodyScanNode::BodyScanNode()
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
        s = ofRandom(100.f, 300.f);
    }
}

BodyScanNode::~BodyScanNode()
{
    spd.clear();
    axis.clear();
    vertices.clear();
    initialVertices.clear();
    vbo.clear();
}

void BodyScanNode::update()
{
    for (int i=0; i<kNumVertices; i++) {
        auto iv = initialVertices.at(i);
        auto& v = vertices.at(i);
        v = iv.rotate(ofGetElapsedTimef() * spd.at(i), axis.at(i));
    }
    vbo.updateVertexData(&vertices.at(0), kNumVertices);
}

void BodyScanNode::customDraw()
{
    vbo.draw(GL_POINTS, 0, vertices.size());
}

#pragma mark ___________________________________________________________________
void SceneBodyScan::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
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

    mCam.enableMouseInput();
}

void SceneBodyScan::exit()
{
    dpDebugFunc();

    mCam.disableMouseInput();
}

void SceneBodyScan::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        for (int i=0; i<getNumSkeletons(); i++) {
            for (auto& mn : getSkeleton(i)->getJoints())
                mn.update();
        }
    }
}

void SceneBodyScan::draw()
{
    mCam.begin();
    
    for (int i=0; i<getNumSkeletons(); i++) {
        ofPushMatrix();
        alignedTranslate(getLineUped(kW, i, getNumSkeletons()), -300.f, 0.f);
        glPointSize(4.f);
        ofSetColor(255, 128);
        ofSetLineWidth(1.5f);
        for (auto& n : getSkeleton(i)->getJoints()) {
            n.draw();
            //if (!n.getParent()) continue;
            //ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
        }
        ofPopMatrix();
    }
    mCam.end();
    
    const float w = kW * 2.f;
    
    const float span = 2.f;
    const float t = ::fmodf(ofGetElapsedTimef(), span) / span;
    const float x = t * kW*3.f;
    ofPushMatrix();
    ofTranslate(kW-x, 0.f);
    ofFill();
    ofSetColor(ofColor::black);
    ofRect(-kW*2.f, 0.f, kW*2.f, kH);
    ofRect(w, 0.f, kW*3.f, kH);
    glBegin(GL_QUADS);
    glColor4f(0.f, 0.f, 0.f, 0.3f); glVertex2f(0.f, 0.f);
    glColor4f(0.f, 0.f, 0.f, 0.3f); glVertex2f(0.f, kH);
    glColor4f(0.f, 0.f, 0.f, 1.0f); glVertex2f(w, kH);
    glColor4f(0.f, 0.f, 0.f, 1.0f); glVertex2f(w, 0.f);
    glEnd();
    ofSetColor(color::kMain, 255);
    ofSetLineWidth(2.f);
    alignedLine(0.f, 20.f, 0.f, kH);
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneBodyScan::updateSkeleton(SkeletonPtr skl)
{
    for (auto& mn : skl->getJoints()) {
        mn.setPosition(mn.getPosition()*4.5f);
    }
}

DP_SCORE_NAMESPACE_END