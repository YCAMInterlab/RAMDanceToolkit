//
//  dpScoreSceneBodyLines.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/11/15.
//
//

#include "dpScoreSceneBodyLines.h"

DP_SCORE_NAMESPACE_BEGIN

BodyLinesNode::BodyLinesNode()
{
}

void BodyLinesNode::setupPoints()
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

void BodyLinesNode::setupLines()
{
    verticesLines.clear();
    verticesColorsW.clear();
    verticesColorsR.clear();
    ofFloatColor white = ofColor::white;
    ofFloatColor red = color::kMain;
    
    auto verts = vertices;
    
    if (getParent()) {
        BodyLinesNode* parent = dynamic_cast<BodyLinesNode*>(getParent());
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

void BodyLinesNode::updatePoints()
{
    for (int i=0; i<kNumVertices; i++) {
        auto iv = initialVertices.at(i) * scale;
        auto& v = vertices.at(i);
        v = iv.rotate(ofGetElapsedTimef() * spd.at(i), axis.at(i));
    }
    vbo.updateVertexData(&vertices.at(0), kNumVertices);
}

void BodyLinesNode::updateLines(bool focus)
{
    verticesLines.clear();
    
    auto verts = vertices;
    for (auto& v : verts) v = getGlobalTransformMatrix().preMult(v);
    
    if (getParent()) {
        BodyLinesNode* parent = dynamic_cast<BodyLinesNode*>(getParent());
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

void BodyLinesNode::draw(bool focus)
{
    ofPushStyle();
    glPointSize(3.f);
    focus ? ofSetColor(color::kMain, 100) : ofSetColor(ofColor::white, 100);
    transformGL();
    vbo.draw(GL_POINTS, 0, vertices.size());
    restoreTransformGL();
    
    ofSetLineWidth(1.f);
    vboLines.draw(GL_LINES, 0, verticesLines.size());
    
    if (focus) {
        windowPos = alignedVec3f(project(getGlobalPosition()));
        windowPos.z = 0.f;
    }
    ofPopStyle();
}

void BodyLinesNode::drawHUD(bool focus)
{
    if (focus) {
        stringstream ss;
        ss << getGlobalPosition() << endl;
        ss << getGlobalOrientation() << endl;
        
        ofPushStyle();
        ofSetColor(color::kMain, 255);
        ofSetLineWidth(1.f);
        float w0 = 400.f;
        float w1 = 20.f;
        float h = -200.f;
        if (windowPos.y < kH * 0.5f) {
            h *= -1.f;
        }
        bool invert = false;
        if (windowPos.x > kW * 0.5f) {
            w0 = kW - w0;
            w1 = kW - w1;
            invert = true;
        }
        ofLine(windowPos, ofVec3f(w0, windowPos.y + h, 0.f));
        ofLine(ofVec3f(w0, windowPos.y + h, 0.f), ofVec3f(w1, windowPos.y + h, 0.f));
        
        if (invert) {
            const float x0 = ofxMot::getJointNameLower(id).size() * 8.f;
            auto strs = ofSplitString(ss.str(), "\n");
            int longest = 0;
            for (auto& s : strs) {
                if (s.length() > longest) longest = s.length();
            }
            const float x1 = longest * 8.f;
            ofSetColor(color::kMain, 255);
            ofDrawBitmapString(ofxMot::getJointNameLower(id), ofPoint(w1 - x0, windowPos.y + h - 4.f));
            ofDrawBitmapString(ss.str(), ofPoint( w1 - x1, windowPos.y + h + 12.f));
        }
        else {
            ofSetColor(color::kMain, 255);
            ofDrawBitmapString(ofxMot::getJointNameLower(id), ofPoint(w1, windowPos.y + h - 4.f));
            ofDrawBitmapString(ss.str(), ofPoint(w1, windowPos.y + h + 12.f));
        }
        
        
        ofPopStyle();
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyLines::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
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
    
    mCam.enableMouseInput();
}

void SceneBodyLines::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneBodyLines::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner) {
        mFrameNum++;
        
        if (mFrameNum %60 == 0) {
            (++mFocusNode) %= getSkeleton(0)->getNumJoints();
        }
    }
}

void SceneBodyLines::draw()
{
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        
        mCam.begin();
        ofPushMatrix();
        ofNoFill();
        const int n = getNumSkeletons();
        const float step = kW/n;
        ofTranslate(-kW*0.5f + step * 0.5f + step * i , -300.f, 0.f);
        
        for(auto& mn : skl->getJoints()) {
            isFocus(mn.id) ? mn.draw(true) : mn.draw(false);
        }
        ofPopMatrix();
        mCam.end();
        
        for(auto& mn : skl->getJoints()) {
            isFocus(mn.id) ? mn.drawHUD(true) : mn.drawHUD(false);
        }
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyLines::setupSkeleton(SkeletonPtr skl)
{
    for (auto& mn : skl->getJoints()) mn.setupPoints();
    for (auto& mn : skl->getJoints()) mn.setupLines();
}

void SceneBodyLines::updateSkeleton(SkeletonPtr skl)
{
    for (auto& mn : skl->getJoints()) {
        mn.setPosition(mn.getPosition()*4.5f);
    }
    
    for (auto& mn : skl->getJoints()) {
        const float t = ofNoise(ofGetElapsedTimef()*1.f + mn.id * 5.f);
        mn.scale = ofMap(t, 0.f, 1.f, 5.f, 130.f);
        mn.updatePoints();
    }
    for (auto& mn : skl->getJoints()) {
        isFocus(mn.id) ? mn.updateLines(true) : mn.updateLines(false);
    }
}

#pragma mark ___________________________________________________________________
bool SceneBodyLines::isFocus(int nodeId)
{
    return (nodeId == mFocusNode && mFrameNum % kFocusLoop < kFocusLoop/2);
}
DP_SCORE_NAMESPACE_END