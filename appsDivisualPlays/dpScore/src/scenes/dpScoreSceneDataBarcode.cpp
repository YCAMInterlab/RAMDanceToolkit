//
//  dpScoreSceneDataBarcode.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#include "dpScoreSceneDataBarcode.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneDataBarcode::initialize()
{
    dpDebugFunc();
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Threshould", 0.f, 2.f, &mThreshould);
}

void SceneDataBarcode::shutDown()
{
    dpDebugFunc();
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataBarcode::enter()
{
    dpDebugFunc();
    mBuffer.clear();
}

void SceneDataBarcode::exit()
{
    dpDebugFunc();
    mBuffer.clear();
}

void SceneDataBarcode::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<kNumCameraunitVectors; i++) {
            if (m.getNumArgs() <= i*2+0 || m.getNumArgs() <= i*2+1) break;
            ofVec2f v(clamp(m.getArgAsFloat(i*2+0)), clamp(m.getArgAsFloat(i*2+1)));
            v.x = v.x * 0.5f + 0.5f;
            v.y = v.y * 0.5f + 0.5f;
            mBuffer.push_back(v);
            const int bufferLen{kW/kBarWidth};
            while (mBuffer.size()>bufferLen) {
                mBuffer.pop_front();
            }
        }
    }
}

void SceneDataBarcode::draw()
{
    ofFill();
    for (int i=0; i<mBuffer.size(); i++) {
        const auto& v = mBuffer.at(i);
        ofPushMatrix();
        alignedTranslate(i * kBarWidth, 20.f);
        
        auto drawBar = [&](float f, float y)
        {
            f >= mThreshould ? ofSetColor(ofColor::white) : ofSetColor(ofColor::black);
            alignedRect(0.f, y, kBarWidth, kHalfH);
        };
        
        drawBar(v.x, 0.f);
        drawBar(v.y, kHalfH);
        
        ofPopMatrix();
    }
}

DP_SCORE_NAMESPACE_END