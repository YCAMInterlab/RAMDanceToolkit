//
//  dpScoreVec2SimpleGraph.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreVec2SimpleGraph.h"


void dpScoreVec2SimpleGraph::initialize()
{
    dpDebugFunc();
    
    mSensorScale = 0.3f;
    mStep = 4;
    mBuffer.clear();
    mBuffer.assign(kW/mStep, ofVec2f::zero());
 
    const string name = dpGetClassName(*this);
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(name);
    mUICanvas->addLabel(name);
    mUICanvas->addSpacer();
    mUICanvas->addIntSlider("Step", 1, 10, &mStep);
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void dpScoreVec2SimpleGraph::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = NULL;
    }
}

void dpScoreVec2SimpleGraph::enter()
{
    dpDebugFunc();
}

void dpScoreVec2SimpleGraph::exit()
{
    dpDebugFunc();
}

void dpScoreVec2SimpleGraph::update(ofxEventMessage& m)
{
    if (m.getAddress() == dpScoreMessageVec2) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mBuffer.push_back(mVec);
        while (mBuffer.size()>kW/mStep-mStep*2) {
            mBuffer.pop_front();
        }
    }
}

void dpScoreVec2SimpleGraph::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofPushMatrix();
    
    ofSetColor(ofColor::white, 255);
    
    ofDrawBitmapString("Dividual Plays System Score - A", 12.f, 16.f);
    
    ofPushMatrix();
    ofTranslate(0.f, dpAlign(20.f));
    ofLine(10.f, 0.f, kW-10.f, 0.f);
    const float step = kW / 210;
    for (int i=0; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(dpAlign(10.f+i*step), 0.f, dpAlign(10.f+i*step), height);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, dpAlign(kH-10.f));
    ofLine(10.f, 0.f, kW-10.f, 0.f);
    for (int i=0; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(dpAlign(10.f+i*step), 0.f, dpAlign(10.f+i*step), -height);
    }
    ofPopMatrix();
    
    const float stepY = (kH - (10.f + 20.f + 40.f)) / 100.f;
    for (int i=0; i<=100; i++) {
        int width = 5.f;
        if (i%10 == 0) width = 8;
        ofLine(0.f, dpAlign(40.f+i*stepY), width, dpAlign(40.f+i*stepY));
    }
    
    ofSetColor(ofColor::white, 150);
    ofTranslate(20.f, dpAlign(kH*0.5f+5.f));
    ofLine(0.f, 0.f, kW, 0.f);
    
    const float mult = kH / 40.f * mSensorScale;
    if (mBuffer.size()>=2) {
        
        ofSetColor(ofColor::white, 255);
        for (int i=0; i<mBuffer.size()-1; i++) {
            ofVec2f v0 = mBuffer.at(i);
            ofVec2f v1 = mBuffer.at(i+1);
            v0 *= mult;
            v1 *= mult;
            ofLine(dpAlign(i*mStep), dpAlign(v0.x),
                   dpAlign((i+1)*mStep), dpAlign(v1.x));
            ofLine(dpAlign(i*mStep), dpAlign(v0.y),
                   dpAlign((i+1)*mStep), dpAlign(v1.y));
        }
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0.f, dpAlign(kH*0.5f+5.f));
    ofSetColor(255, 0, 0, 255);
    ofSetLineWidth(2.f);
    const ofVec2f v = mVec * mult;
    ofLine(ofGetWidth()-14.f, dpAlign(v.x), ofGetWidth(), dpAlign(v.x));
    ofLine(ofGetWidth()-14.f, dpAlign(v.y), ofGetWidth(), dpAlign(v.y));
    ofPopMatrix();
    ofPopStyle();
}
