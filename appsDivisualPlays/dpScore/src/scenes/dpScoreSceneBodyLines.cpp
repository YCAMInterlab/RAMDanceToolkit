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
    verticesColorsW.clear();
    verticesColorsR.clear();
    ofFloatColor white = ofColor::white;
    ofFloatColor red = color::kMain;
    
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
                verticesColorsW.push_back(ofFloatColor(white, 0.02f));
                verticesColorsW.push_back(ofFloatColor(white, 0.06f));
                verticesColorsR.push_back(ofFloatColor(red, 0.02f));
                verticesColorsR.push_back(ofFloatColor(red, 0.06f));
            }
            else {
                verticesColorsW.push_back(ofFloatColor(white, 0.06f));
                verticesColorsW.push_back(ofFloatColor(white, 0.02f));
                verticesColorsR.push_back(ofFloatColor(red, 0.06f));
                verticesColorsR.push_back(ofFloatColor(red, 0.02f));
            }
        }
    }
    
    vboLines.setVertexData(&verticesLines.at(0), verticesLines.size(), GL_DYNAMIC_DRAW);
    vboLines.setColorData(&verticesColorsW.at(0), verticesColorsW.size(), GL_DYNAMIC_DRAW);
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

void SceneBodyLines::Node::updateLines(bool focus)
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
    if (pFocus != focus) {
        if (focus)
            vboLines.updateColorData(&verticesColorsR.at(0), verticesColorsR.size());
        else
            vboLines.updateColorData(&verticesColorsW.at(0), verticesColorsW.size());
    }
    
    pFocus = focus;
}

void SceneBodyLines::Node::draw(bool focus)
{
    ofPushStyle();
    glPointSize(2.f);
    focus ? ofSetColor(color::kMain, 100) : ofSetColor(ofColor::white, 100);
    transformGL();
    vbo.draw(GL_POINTS, 0, vertices.size());
    restoreTransformGL();
    
    ofSetLineWidth(1.f);
    vboLines.draw(GL_LINES, 0, verticesLines.size());
    
    if (focus) {
        windowPos = project(getGlobalPosition());
        windowPos.x = alignf(windowPos.x);
        windowPos.y = alignf(windowPos.y);
        windowPos.z = 0.f;
    }
    ofPopStyle();
}

void SceneBodyLines::Node::drawHUD(bool focus)
{
    if (focus) {
        stringstream ss;
        //ss << ofxMot::getJointNameLower(id) << endl;
        ss << getGlobalPosition() << endl;
        ss << getGlobalOrientation() << endl;
        
        ofPushStyle();
        ofSetColor(color::kMain, 255);
        ofSetLineWidth(1.f);
        float w0 = -150.f;
        float w1 = -(kW * 0.5 - ::fabsf(kW*0.5f - windowPos.x) - 50.f);
        float h = -50.f;
        bool invert = false;
        if (windowPos.x > kW * 0.5f) {
            w0 *= -1.f;
            w1 *= -1.f;
            invert = true;
        }
        ofLine(windowPos, ofVec3f(windowPos.x + w0, windowPos.y + h, 0.f));
        ofLine(ofVec3f(windowPos.x + w0, windowPos.y + h, 0.f), ofVec3f(windowPos.x + w1, windowPos.y + h, 0.f));
        
        if (invert) {
            ofSetColor(color::kMain, 255);
            //ofSetColor(ofColor::white, 255);
            ofDrawBitmapString(ofxMot::getJointNameLower(id), ofPoint(windowPos.x + w0 + 300.f, windowPos.y + h - 4.f));
            //ofSetColor(color::kMain, 255);
            ofDrawBitmapString(ss.str(), ofPoint(windowPos.x + w0 + 300.f, windowPos.y + h + 12.f));
        }
        else {
            ofSetColor(color::kMain, 255);
            //ofSetColor(ofColor::white, 255);
            ofDrawBitmapString(ofxMot::getJointNameLower(id), ofPoint(windowPos.x + w1, windowPos.y + h - 4.f));
            //ofSetColor(color::kMain, 255);
            ofDrawBitmapString(ss.str(), ofPoint(windowPos.x + w1, windowPos.y + h + 12.f));
        }
         
        
        ofPopStyle();
    }
}

#pragma mark ___________________________________________________________________

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
    mFrameNum++;
    
    if (mFrameNum %60 == 0) {
        (++mFocusNode) %= mSkeleton->getNumJoints();
    }
    
    if (m.getAddress() == kAddrMotioner && mSkeleton) {
        for (auto& mn : mSkeleton->getJoints()) {
            const float t = ofNoise(ofGetElapsedTimef()*1.f + mn.id * 5.f);
            mn.scale = ofMap(t, 0.f, 1.f, 1.f, 30.f);
            mn.updatePoints();
        }
        for (auto& mn : mSkeleton->getJoints()) {
            isFocus(mn.id) ? mn.updateLines(true) : mn.updateLines(false);
        }
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
            isFocus(mn.id) ? mn.draw(true) : mn.draw(false);
        }
    }
    
    ofPopMatrix();
    mCam.end();
    
    if (mSkeleton) {
        for(auto& mn : mSkeleton->getJoints()) {
            isFocus(mn.id) ? mn.drawHUD(true) : mn.drawHUD(false);
        }
    }
    
    drawHeader();
    
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

bool SceneBodyLines::isFocus(int nodeId)
{
    return (nodeId == mFocusNode && mFrameNum % kFocusLoop < kFocusLoop/2);
}
DP_SCORE_NAMESPACE_END