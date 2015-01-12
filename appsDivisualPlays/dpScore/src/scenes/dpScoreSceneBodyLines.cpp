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
        v = randVec3f() * scale * ofRandom(0.5f, 1.f);
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

void SceneBodyLines::Node::setupLines()
{
    verticesLines.clear();
    
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
        }
    }
    
    vboLines.setVertexData(&verticesLines.at(0), verticesLines.size(), GL_DYNAMIC_DRAW);
}

void SceneBodyLines::Node::update()
{
    for (int i=0; i<kNumVertices; i++) {
        auto iv = initialVertices.at(i);
        auto& v = vertices.at(i);
        v = iv.rotate(ofGetElapsedTimef() * spd.at(i), axis.at(i));
    }
    vbo.updateVertexData(&vertices.at(0), kNumVertices);
    
    verticesLines.clear();
    for (int j=0; j<vertices.size(); j++) {
        const ofVec3f v0 = vertices.at(j);
        for (int i=(j+1); i<vertices.size(); i++) {
            const ofVec3f v1 = vertices.at(i);
            verticesLines.push_back(v0);
            verticesLines.push_back(v1);
        }
    }
    
    vboLines.updateVertexData(&verticesLines.at(0), verticesLines.size());
}

void SceneBodyLines::Node::draw()
{
    glPointSize(3.f);
    ofSetColor(255, 64);
    vbo.draw(GL_POINTS, 0, vertices.size());

    ofSetColor(255, 32);
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
    mUICanvas->addSlider("Sphere Scale", 200.f, 600.f, &mScale);
    
    mNodes.assign(ofxMot::NUM_JOINTS, Node());
    ofxMot::createTree(mNodes);
    
    for (auto& mn : mNodes) {
        mn.setupPoints();
    }
    for (auto& mn : mNodes) {
        mn.setupLines();
    }
    
    mCam.setDistance(200);
    
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
}

void SceneBodyLines::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyLines::onUpdateSkeleton);
}

void SceneBodyLines::update(ofxEventMessage& m)
{
    ofSetWindowTitle(getName() + ": " + ofToString(ofGetFrameRate(), 2));
    
    if (m.getAddress() == kAddrMotioner) {
//        for (auto& mn : mNodes) mn->update();
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
        auto& joints = mSkeleton->getJoints();
        //for (int i=0; i<joints.size(); i++) {
        for (int i=0; i<joints.size(); i++) {
            auto& n = joints.at(i);
            
            n.transformGL();
            mNodes.at(i).draw();
            n.restoreTransformGL();
            
            //if (!n.getParent()) continue;
            //ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
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
        mSkeleton = skl;
    }
}

DP_SCORE_NAMESPACE_END