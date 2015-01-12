//
//  dpScoreSceneVec2Clocks.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2Clocks.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneVec2Clocks::initialize()
{
    dpDebugFunc();
    
    mClockBuffer.clear();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 5.0f, &mSensorScale);
}

void SceneVec2Clocks::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneVec2Clocks::enter()
{
    dpDebugFunc();
}

void SceneVec2Clocks::exit()
{
    dpDebugFunc();
}

void SceneVec2Clocks::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrVec2) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mClockBuffer.push_back(mVec);
        while (mClockBuffer.size()>_clockNumX * _clockNumY) {
            mClockBuffer.pop_front();
        }
    }
}

void SceneVec2Clocks::draw()
{
    ofPushStyle();
    
    ofSetCircleResolution(64);
    
    ofEnableAlphaBlending();
    
    drawHeader();
    
    ofNoFill();
    const float step = 10.f;
    ofSetColor(255, 50);
    
    for (int i=0; i<=kW/step; i++) {
        ofLine(alignf(i*step), 20.f, alignf(i*step), alignf(kH-30.f));
    }
    
    for (int j=2; j<kH/step-2; j++) {
        ofLine(0.f, alignf(j*step), alignf(kW), alignf(j*step));
    }
    
    const float circleStep = 80.f;
    ofSetColor(255, 255);
    for (int j=0; j<_clockNumY; j++) {
        ofPushMatrix();
        ofTranslate(0.f, 30.f + j * circleStep + 30.f, 0.f);
        
        for (int i=0; i<_clockNumX; i++) {
            ofPushMatrix();
            ofTranslate(30.f + i * circleStep + 10.f, 0.f, 0.f);
            
            ofSetLineWidth(1.f);
            ofSetColor(255, 150);
            ofCircle(0.f, 0.f, 30.f);
            
            const int idx = (_clockNumX * _clockNumY - 1) - (i + j * _clockNumX);
            
            (i == 0 && j == 0) ? ofSetColor(color::kMain, 255) : ofSetColor(ofColor::white, 255);
            
            if (idx < mClockBuffer.size()) {
                ofVec2f v = mClockBuffer.at(idx);
                //v.normalize();
                const float mult = kH / 150.f * mSensorScale;
                
                ofSetLineWidth(1.5f);
                ofLine(ofVec2f::zero(), v * mult);
                //ofLine(ofVec2f::zero(), v * 30.f);
            }
            
            ofSetColor(255, 100);
            ofSetLineWidth(1.f);
            const float size = 5.f;
            ofLine(alignf(-size), 0.5f, alignf(size), 0.5f);
            ofLine(0.5f, alignf(-size), 0.5f, alignf(size));
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    ofPopStyle();

}

DP_SCORE_NAMESPACE_END