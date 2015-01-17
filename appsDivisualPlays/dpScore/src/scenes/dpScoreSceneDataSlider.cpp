//
//  dpScoreSceneDataSlider.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#include "dpScoreSceneDataSlider.h"

DP_SCORE_NAMESPACE_BEGIN

SceneDataSlider::Slider::Slider()
{
}

void SceneDataSlider::Slider::update(const ofVec2f& v)
{
    vec = v;
    vec.x = clamp(v.x);
    vec.y = clamp(v.y);
    const float f = 0.f;
    vec = vec * (1.f - f) + prevVec * f;
    
    prevVec = vec;
}

void SceneDataSlider::Slider::draw(SceneDataSlider* owner, float w)
{
    const float hx{::fabsf(vec.x) * (kH-30.f)};
    const float hy{::fabsf(vec.y) * (kH-30.f)};
    
    //const int precision = 13;
    const string sx{ofToString(vec.x)};
    const string sy{ofToString(vec.y)};
    const float sxw{owner->mFont.stringWidth(sx)};
    const float syw{owner->mFont.stringWidth(sy)};
    
    ofFill();
    //const float ax{::fabsf(vec.x) * 255.f};
    //const float ay{::fabsf(vec.y) * 255.f};
    ofSetColor(ofColor::white, 64);
    alignedRect(0.f, kH, w*0.5f, -hx);
    //ofSetColor(ofColor::white, ay);
    alignedRect(w*0.5f, kH, w*0.5f, -hy);

    ofNoFill();
    ofSetLineWidth(1.f);
    ofSetColor(ofColor::white, 256);
    alignedRect(0.f, kH, w*0.5f, -hx);
    alignedRect(w*0.5f, kH, w*0.5f, -hy);
    
    ofSetColor(128, 256);
    alignedRect(1.f, 30.f, w-1.f, kH-31.f);
    
    
    ofSetColor(ofColor::white, 256);
    ofPushMatrix();
    alignedTranslate(5.f, kH-hx+5.f);
    ofRotate(90.f);
    owner->mFont.drawString(sx, 0.f, 0.f);
    ofPopMatrix();
    
    ofPushMatrix();
    alignedTranslate(5.f+w*0.5f, kH-hy+5.f);
    ofRotate(90.f);
    owner->mFont.drawString(sy, 0.f, 0.f);
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneDataSlider::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
}

void SceneDataSlider::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataSlider::enter()
{
    dpDebugFunc();
    
    mSliders.assign(kNumCameraunitVectors, ofPtr<Slider>());
    for (auto& p : mSliders) {
        p = ofPtr<Slider>(new Slider());
    }
    
    mFont.loadFont(kFontPath, 26);
}

void SceneDataSlider::exit()
{
    dpDebugFunc();
    mSliders.clear();
    
    mFont = ofTrueTypeFont();
}

void SceneDataSlider::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<kNumCameraunitVectors ; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
            mSliders.at(i)->update(v);
        }
    }
}

void SceneDataSlider::draw()
{
    for (int i=0; i<mSliders.size(); i++) {
        ofPushMatrix();
        const float step{kW/(float)mSliders.size()};
        alignedTranslate(i*step, 0.f);
        mSliders.at(i)->draw(this, step);
        ofPopMatrix();
    }
}

DP_SCORE_NAMESPACE_END