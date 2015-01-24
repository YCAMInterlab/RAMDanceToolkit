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
        s = ofRandom(200.f, 400.f);
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

void BodyScanNode::drawPointer()
{
    stringstream ss;
    ss << getGlobalPosition() << endl;
    ss << getGlobalOrientation() << endl;
    const float t{ofGetElapsedTimef()};
    
    ofPushStyle();
    ofSetColor(color::kMain, 255);
    ofSetLineWidth(1.f);
    float w0{400.f};
    float w1{20.f};
    float h{-200.f};
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
        const float x0{ofxMot::getJointNameLower(id).size() * 8.f};
        auto strs = ofSplitString(ss.str(), "\n");
        int longest{0};
        for (auto& s : strs) {
            if (s.length() > longest) longest = s.length();
        }
        const float x1{longest * 8.f};
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

void BodyScanNode::customDraw()
{
    ofPushMatrix();
    ofScale(scaleAdjust, scaleAdjust, scaleAdjust);
    vbo.draw(GL_POINTS, 0, vertices.size());
    ofPopMatrix();
    restoreTransformGL();
    windowPos = alignedVec3f(project(getGlobalPosition()));
    windowPos.z = 0.f;
    transformGL();
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
    
    mEnterTime = ofGetElapsedTimef();
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
    const float time{ofGetElapsedTimef() - mEnterTime};
    
    const float pTotal{12.f};
    const float p0{10.f};
    const float p1{pTotal - p0};
    const float phase{::fmodf(time, pTotal)};
    
    if (phase < p0) {
        mCam.begin();
        
        for (int i=0; i<getNumSkeletons(); i++) {
            ofPushMatrix();
            alignedTranslate(getLineUped(kW, i, getNumSkeletons()), -300.f, 0.f);
            glPointSize(4.f);
            ofSetColor(255, 128);
            ofSetLineWidth(1.5f);
            for (auto& n : getSkeleton(i)->getJoints()) {
                n.scaleAdjust = 1.f;
                n.draw();
            }
            ofPopMatrix();
        }
        mCam.end();
        
        const float w = kW * 2.f;
        
        const float span = 2.f;
        const float t = ::fmodf(time, span) / span;
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
        mCam.end();
    }
    else {
        const float t{phase - p0};
        const int joint{(int)(t / p1 * ofxMot::NUM_JOINTS)};
        for (int i=0; i<getNumSkeletons(); i++) {
            auto& n = getSkeleton(i)->getJoint(joint);
            mCam.begin();
            ofPushMatrix();
            alignedTranslate(getLineUped(kW, i, getNumSkeletons()), -300.f, 0.f);
            glPointSize(4.f);
            ofSetColor(255, 128);
            ofSetLineWidth(1.5f);
            n.scaleAdjust = 0.5f;
            n.draw();
            ofPopMatrix();
            mCam.end();
            n.drawPointer();
        }
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyScan::updateSkeleton(SkeletonPtr skl)
{
    for (auto& mn : skl->getJoints()) {
        mn.setPosition(mn.getPosition()*4.5f);
    }
}

DP_SCORE_NAMESPACE_END