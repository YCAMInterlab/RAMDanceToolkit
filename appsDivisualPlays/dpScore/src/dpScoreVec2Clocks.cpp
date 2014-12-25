//
//  dpScoreVec2Clocks.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreVec2Clocks.h"

static const int _clockNumX = 16;
static const int _clockNumY = 9;

void dpScoreVec2Clocks::initialize()
{
    dpDebugFunc();
    
    mSensorScale = 0.3f;
    mClockBuffer.clear();
    
    const string name = dpGetClassName(*this);
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(name);
    mUICanvas->addLabel(name);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void dpScoreVec2Clocks::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = NULL;
    }
}

void dpScoreVec2Clocks::enter()
{
    dpDebugFunc();
}

void dpScoreVec2Clocks::exit()
{
    dpDebugFunc();
}

void dpScoreVec2Clocks::update(ofxEventMessage& m)
{
    if (m.getAddress() == dpScoreMessageVec2) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mClockBuffer.push_back(mVec);
        while (mClockBuffer.size()>_clockNumX * _clockNumY) {
            mClockBuffer.pop_front();
        }
    }
}

void dpScoreVec2Clocks::draw()
{
    ofPushStyle();
    
    ofSetCircleResolution(64);
    
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - B", 12.f, 16.f);
    
    ofNoFill();
    const float step = 10.f;
    ofSetColor(255, 50);
    
    for (int i=0; i<=kW/step; i++) {
        ofLine(dpAlign(i*step), 20.f, dpAlign(i*step), dpAlign(kH-30.f));
    }
    
    for (int j=2; j<kH/step-2; j++) {
        ofLine(0.f, dpAlign(j*step), dpAlign(kW), dpAlign(j*step));
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
            
            (i == 0 && j == 0) ? ofSetColor(255, 0, 0, 255) : ofSetColor(255, 255);
            
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
            ofLine(dpAlign(-size), 0.5f, dpAlign(size), 0.5f);
            ofLine(0.5f, dpAlign(-size), 0.5f, dpAlign(size));
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    ofPopStyle();

}