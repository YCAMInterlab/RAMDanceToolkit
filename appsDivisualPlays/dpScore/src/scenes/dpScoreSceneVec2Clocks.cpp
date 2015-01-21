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
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.0f, &mSensorScale);
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
    
    mClockBuffer.clear();
}

void SceneVec2Clocks::exit()
{
    dpDebugFunc();
    
    mClockBuffer.clear();
}

void SceneVec2Clocks::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        mVec.x = clamp(m.getArgAsFloat(0));
        mVec.y = clamp(m.getArgAsFloat(1));
        mClockBuffer.push_back(mVec.interpolate(mPrevVec, 0.5f));
        mClockBuffer.push_back(mVec);
        mPrevVec = mVec;
        while (mClockBuffer.size()>_clockNumX * _clockNumY) {
            mClockBuffer.pop_front();
        }
    }
}

void SceneVec2Clocks::draw()
{
    ofSetCircleResolution(64);
    
    const float step{20.f};
    ofSetColor(255, 70);
    ofSetLineWidth(1.f);
    
    for (int i=0; i<=kW/step; i++) {
        alignedLine(i*step, 60.f, i*step, kH-60.f);
    }
    
    for (int j=3; j<kH/step-2; j++) {
        alignedLine(0.f, j*step, kW, j*step);
    }
    
    const float circleStep{120.f};
    const float r{40.f};
    
    ofSetColor(255, 255);
    for (int j=0; j<_clockNumY; j++) {
        ofPushMatrix();
        ofTranslate(0.f, 30.f + j * circleStep + 90.f, 0.f);
        
        for (int i=0; i<_clockNumX; i++) {
            ofPushMatrix();
            ofTranslate(30.f + i * circleStep + 30.f, 0.f, 0.f);
            
            ofSetLineWidth(1.5f);
            ofSetColor(128);
            ofCircle(0.f, 0.f, r);
            
            const int idx{(_clockNumX * _clockNumY - 1) - (i + j * _clockNumX)};
            
            if (i == 0 && j == 0) {
                ofSetLineWidth(3.f);
                ofSetColor(color::kMain, 255);
            }
            else {
                ofSetLineWidth(1.5f);
                ofSetColor(ofColor::white, 255);
            }
            
            if (idx < mClockBuffer.size()) {
                ofVec2f v{mClockBuffer.at(idx)};
                const float mult{r * 2.f * mSensorScale};
                ofLine(ofVec2f::zero(), v * mult);
            }
            
            ofSetColor(150);
            ofSetLineWidth(1.f);
            const float size{10.f};
            alignedLine(-size, 0.f, size, 0.f);
            alignedLine(0.f, -size, 0.f, size);
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
}

DP_SCORE_NAMESPACE_END