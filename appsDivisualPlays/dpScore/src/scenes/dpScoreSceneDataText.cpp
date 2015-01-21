//
//  dpScoreSceneDataText.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#include "dpScoreSceneDataText.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneDataText::initialize()
{
    dpDebugFunc();
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
}

void SceneDataText::shutDown()
{
    dpDebugFunc();
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataText::enter()
{
    dpDebugFunc();
    mVectors.clear();
    mFont.loadFont(kFontPath, kFontSize);
}

void SceneDataText::exit()
{
    dpDebugFunc();
    mVectors.clear();
    mFont = ofTrueTypeFont();
}

void SceneDataText::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        mVectors.clear();
        for (int i=0; i<kNumCameraunitVectors; i++) {
            if (m.getNumArgs() <= i*2+0 || m.getNumArgs() <= i*2+1) break;
            ofVec2f v(m.getArgAsFloat(i*2+0), m.getArgAsFloat(i*2+1));
            mVectors.push_back(v);
        }
    }
}

void SceneDataText::draw()
{
    ofFill();
    const float h{mFont.stringHeight("-1,234567890")};
    const int precision{13};
    const float s{12.f};
    const float w{mFont.stringWidth(ofToString(1.f, precision))};
    const float minusW{mFont.stringWidth("-")};
    const string comma{","};
    const float commaW{mFont.stringWidth(comma)};
    
    for (int i=0; i<mVectors.size(); i++) {
        const auto& v = mVectors.at(i);
        const string sx{ofToString(::fabsf(v.x), precision)};
        const string sy{ofToString(::fabsf(v.y), precision)};
        ofPushMatrix();
        alignedTranslate(20.f, h + 30.f + (h + s)*i);
        const float ax{255 * ::fabsf(v.x)};
        ofSetColor(ofColor::white, ax);
        if (v.x<0.f) mFont.drawString("-", 0.f, 0.f);
        alignedTranslate(minusW + s, 0.f);
        mFont.drawString(sx, 0.f, 0.f);
        ofSetColor(ofColor::gray, 255);
        alignedTranslate(w + s*2.f, 0.f);
        mFont.drawString(comma, 0.f, 0.f);
        alignedTranslate(commaW + s*4.f, 0.f);
        const float ay{255 * ::fabsf(v.y)};
        ofSetColor(ofColor::white, ay);
        if (v.y<0.f) mFont.drawString("-", 0.f, 0.f);
        alignedTranslate(minusW + s, 0.f);
        mFont.drawString(sy, 0.f, 0.f);
        ofPopMatrix();
    }
    
}

DP_SCORE_NAMESPACE_END