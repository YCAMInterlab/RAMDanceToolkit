//
//  dpScoreSceneDataCircle.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#include "dpScoreSceneDataCircle.h"

DP_SCORE_NAMESPACE_BEGIN

SceneDataCircle::Circle::Circle()
{
    data.assign(kResolution, 0.f);
}

void SceneDataCircle::Circle::update(float f)
{
    data.push_back(clamp(f));
    while (data.size() > kResolution) {
        data.pop_front();
    }
}

void SceneDataCircle::Circle::draw()
{
    const float step = mRadius * TWO_PI / kResolution;
    for (int i=0; i<data.size(); i++) {
        const float f = data.at(i);
        if (::fabsf(f) < 0.01f) continue;
        
        ofPushMatrix();
        const float angle = -360.f / kResolution * i;
        ofRotate(angle);
        ofTranslate(0.f, -mRadius);
        const float h = f * mRadius;
        ofFill();
        ofSetColor(128, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofNoFill();
        ofSetLineWidth(2.f);
        ofSetColor(ofColor::white, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofPopMatrix();
    }
    ofSetLineWidth(step*0.5f);
    ofSetColor(color::kMain, 255);
    ofLine(0.f, -mRadius-50.f, 0.f, -mRadius+50.f);

}

#pragma mark ___________________________________________________________________
void SceneDataCircle::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
}

void SceneDataCircle::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataCircle::enter()
{
    dpDebugFunc();
    
    mCircles.assign(kNumCircles, ofPtr<Circle>());
    for (auto& p : mCircles) {
        p = ofPtr<Circle>(new Circle());
    }
}

void SceneDataCircle::exit()
{
    dpDebugFunc();
}

void SceneDataCircle::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {

        for (int i=0; i<kNumCircles/2; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
            mCircles.at(2*i+0)->update(v.x);
            mCircles.at(2*i+1)->update(v.y);
        }
    }
}

void SceneDataCircle::draw()
{
    ofPushMatrix();
    for (int i=0; i<mCircles.size(); i++) {
        ofPushMatrix();
        ofTranslate(kW*0.5f + getLineUped(kW, i/2, mCircles.size()/2), kH*0.5f);
        mCircles.at(i)->draw();
        ofPopMatrix();
    }
    ofPopMatrix();
}

DP_SCORE_NAMESPACE_END