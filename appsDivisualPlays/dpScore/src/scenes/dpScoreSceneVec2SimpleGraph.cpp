//
//  dpScoreSceneVec2SimpleGraph.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2SimpleGraph.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneVec2SimpleGraph::initialize()
{
    dpDebugFunc();
    
    mBuffer.clear();
    mBuffer.assign(kW/mStep, ofVec2f::zero());
 
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addIntSlider("Step", 1, 10, &mStep);
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void SceneVec2SimpleGraph::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneVec2SimpleGraph::enter()
{
    dpDebugFunc();
}

void SceneVec2SimpleGraph::exit()
{
    dpDebugFunc();
}

void SceneVec2SimpleGraph::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrVec2) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mBuffer.push_back(mVec);
        while (mBuffer.size()>kW/mStep-mStep*2) {
            mBuffer.pop_front();
        }
    }
}

void SceneVec2SimpleGraph::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofPushMatrix();
    
    drawHeader();
    
    ofPushMatrix();
    ofTranslate(0.f, alignf(20.f));
    ofLine(10.f, 0.f, kW-10.f, 0.f);
    const float step = kW / 210;
    for (int i=0; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(alignf(10.f+i*step), 0.f, alignf(10.f+i*step), height);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, alignf(kH-10.f));
    ofLine(10.f, 0.f, kW-10.f, 0.f);
    for (int i=0; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(alignf(10.f+i*step), 0.f, alignf(10.f+i*step), -height);
    }
    ofPopMatrix();
    
    const float stepY = (kH - (10.f + 20.f + 40.f)) / 100.f;
    for (int i=0; i<=100; i++) {
        int width = 5.f;
        if (i%10 == 0) width = 8;
        ofLine(0.f, alignf(40.f+i*stepY), width, alignf(40.f+i*stepY));
    }
    
    ofSetColor(ofColor::white, 150);
    ofTranslate(20.f, alignf(kH*0.5f+5.f));
    ofLine(0.f, 0.f, kW, 0.f);
    
    const float mult = kH / 40.f * mSensorScale;
    if (mBuffer.size()>=2) {
        
        ofSetColor(ofColor::white, 255);
        for (int i=0; i<mBuffer.size()-1; i++) {
            ofVec2f v0 = mBuffer.at(i);
            ofVec2f v1 = mBuffer.at(i+1);
            v0 *= mult;
            v1 *= mult;
            ofLine(alignf(i*mStep), alignf(v0.x),
                   alignf((i+1)*mStep), alignf(v1.x));
            ofLine(alignf(i*mStep), alignf(v0.y),
                   alignf((i+1)*mStep), alignf(v1.y));
        }
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, alignf(kH*0.5f+5.f));
    ofSetColor(255, 0, 0, 255);
    ofSetLineWidth(2.f);
    const ofVec2f v = mVec * mult;
    ofLine(ofGetWidth()-14.f, alignf(v.x), ofGetWidth(), alignf(v.x));
    ofLine(ofGetWidth()-14.f, alignf(v.y), ofGetWidth(), alignf(v.y));
    ofPopMatrix();
    ofPopStyle();
}

DP_SCORE_NAMESPACE_END