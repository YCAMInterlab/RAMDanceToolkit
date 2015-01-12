//
//  dpScoreSceneBodyLines.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/11/15.
//
//

#include "dpScoreSceneBodyLines.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBodyLines::Node::Node()
{
}

void SceneBodyLines::Node::setupPoints()
{
    vertices.assign(kNumVertices, ofVec3f::zero());
    for (auto& v : vertices) {
        v = randVec3f() * ofRandom(0.1f, 1.f);
    }
    initialVertices = vertices;
    
    vbo.setVertexData(&vertices.at(0), vertices.size(), GL_DYNAMIC_DRAW);
    
    axis.assign(kNumVertices, ofVec3f::zero());
    for (auto& a : axis) {
        a = randVec3f();
    }
    
    spd.assign(kNumVertices, 0.f);
    for (auto& s : spd) {
        s = ofRandom(50.f, 200.f);
    }
}

void SceneBodyLines::Node::setupLines()
{
    verticesLines.clear();
    verticesColors.clear();
    
    auto verts = vertices;
    
    if (getParent()) {
        Node* parent = dynamic_cast<Node*>(getParent());
        for (const auto& v : parent->vertices) {
            verts.push_back(v);
        }
    }
    
    for (int j=0; j<verts.size(); j++) {
        const ofVec3f v0 = verts.at(j);
        for (int i=(j+1); i<verts.size(); i++) {
            const ofVec3f v1 = verts.at(i);
            verticesLines.push_back(v0);
            verticesLines.push_back(v1);
            if (j%2==0) {
                verticesColors.push_back(ofFloatColor(1.f, 1.f, 1.f, 0.02f));
                verticesColors.push_back(ofFloatColor(1.f, 1.f, 1.f, 0.06f));
            }
            else {
                verticesColors.push_back(ofFloatColor(1.f, 1.f, 1.f, 0.06f));
                verticesColors.push_back(ofFloatColor(1.f, 1.f, 1.f, 0.02f));
            }
        }
    }
    
    vboLines.setVertexData(&verticesLines.at(0), verticesLines.size(), GL_DYNAMIC_DRAW);
    vboLines.setColorData(&verticesColors.at(0), verticesColors.size(), GL_DYNAMIC_DRAW);
}

void SceneBodyLines::Node::updatePoints()
{
    for (int i=0; i<kNumVertices; i++) {
        auto iv = initialVertices.at(i) * scale;
        auto& v = vertices.at(i);
        v = iv.rotate(ofGetElapsedTimef() * spd.at(i), axis.at(i));
    }
    vbo.updateVertexData(&vertices.at(0), kNumVertices);
}

void SceneBodyLines::Node::updateLines()
{
    verticesLines.clear();
    
    auto verts = vertices;
    for (auto& v : verts) v = getGlobalTransformMatrix().preMult(v);
    
    if (getParent()) {
        Node* parent = dynamic_cast<Node*>(getParent());
        for (auto v : parent->vertices) {
            v = parent->getGlobalTransformMatrix().preMult(v);
            verts.push_back(v);
        }
    }
    
    for (int j=0; j<verts.size(); j++) {
        const ofVec3f v0 = verts.at(j);
        for (int i=(j+1); i<verts.size(); i++) {
            const ofVec3f v1 = verts.at(i);
            verticesLines.push_back(v0);
            verticesLines.push_back(v1);
        }
    }
    vboLines.updateVertexData(&verticesLines.at(0), verticesLines.size());
}

void SceneBodyLines::Node::customDraw()
{
    glPointSize(2.f);
    ofSetColor(255, 100);
    vbo.draw(GL_POINTS, 0, vertices.size());
}

void SceneBodyLines::Node::drawLines()
{
    //ofSetColor(ofColor::red, 64);
    //ofPushMatrix();
    //ofMultMatrix(getGlobalTransformMatrix());
    //ofCircle(ofPoint::zero(), 10.f);
    //ofPopMatrix();
    //
    //if (getParent()) {
    //    ofSetColor(ofColor::blue, 64);
    //    auto* parent = static_cast<Node*>(getParent());
    //    ofPushMatrix();
    //    ofMultMatrix(parent->getGlobalTransformMatrix());
    //    ofDrawBitmapString(ofxMot::getJointNameLower(parent->id), ofPoint::zero());
    //    ofCircle(ofPoint::zero(), 5.f);
    //    ofPopMatrix();
    //}
    
    ofSetColor(255, 255);
    ofSetLineWidth(1.f);
    vboLines.draw(GL_LINES, 0, verticesLines.size());
}

void SceneBodyLines::initialize()
{
    dpDebugFunc();
    
    OFX_BEGIN_EXCEPTION_HANDLING
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mSkeleton = ofxMot::SkeletonBase<Node>::create();
    
    for (auto& mn : mSkeleton->getJoints()) mn.setupPoints();
    for (auto& mn : mSkeleton->getJoints()) mn.setupLines();
    
    mCam.setDistance(200);
    mCam.disableMouseInput();
    
    OFX_END_EXCEPTION_HANDLING
}

void SceneBodyLines::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyLines::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyLines::onUpdateSkeleton);
    mCam.enableMouseInput();
}

void SceneBodyLines::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyLines::onUpdateSkeleton);
    mCam.disableMouseInput();
}

void SceneBodyLines::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner && mSkeleton) {
        int i=0;
        for (auto& mn : mSkeleton->getJoints()) {
            const float t = ofNoise(ofGetElapsedTimef()*1.f + i * 5.f);
            mn.scale = ofMap(t, 0.f, 1.f, 1.f, 30.f);
            mn.updatePoints();
            i++;
        }
        for (auto& mn : mSkeleton->getJoints()) mn.updateLines();
    }
}

void SceneBodyLines::draw()
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
        for(auto& mn : mSkeleton->getJoints()) {
            mn.draw();
            mn.drawLines();
            //if (mn.getParent()) {
            //    ofSetColor(ofColor::red);
            //    ofLine(mn.getGlobalPosition(), mn.getParent()->getGlobalPosition());
            //}
        }
    }
    
    ofPopMatrix();
    mCam.end();
   
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(getName(), 12.f, 16.f);
    
    ofPopMatrix();
    ofPopStyle();
}

#pragma mark ___________________________________________________________________
void SceneBodyLines::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        mSkeleton->copyMatrices(skl);
    }
}

DP_SCORE_NAMESPACE_END