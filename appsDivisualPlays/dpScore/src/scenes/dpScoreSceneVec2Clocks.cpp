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
    if (m.getAddress() == kOscAddrCaneraUnitVector) {
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
    ofSetCircleResolution(64);
    
    const float step = 10.f;
    ofSetColor(255, 50);
    
    for (int i=0; i<=kW/step; i++) {
        alignedLine(i*step, 20.f, i*step, kH-20.f);
    }
    
    for (int j=2; j<kH/step-1; j++) {
        alignedLine(0.f, j*step, kW, j*step);
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
            alignedLine(-size, 0.5f, size, 0.5f);
            alignedLine(0.5f, -size, 0.5f, size);
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
}

DP_SCORE_NAMESPACE_END