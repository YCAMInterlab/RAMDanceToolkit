//
//  dpScoreSceneDataSphere.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/16/15.
//
//

#include "dpScoreSceneDataSphere.h"

DP_SCORE_NAMESPACE_BEGIN

SceneDataSphere::Circle::Circle()
{
    data.assign(kResolution, ofVec2f::zero());
}

void SceneDataSphere::Circle::update(const ofVec2f& v)
{
    data.push_back(ofVec2f(clamp(v.x), clamp(v.y)));
    while (data.size() > kResolution) {
        data.pop_front();
    }
}

void SceneDataSphere::Circle::draw()
{
    const float step = mRadius * TWO_PI / kResolution;
    for (int i=0; i<data.size(); i++) {
        const auto v = data.at(i);
        if (v.length() < 0.01f) continue;
        
        ofPushMatrix();
        ofRotateY(ofGetElapsedTimef()*0.5f);
        
        const float angle{360.f / kResolution * i};
        const float radian{::atan2f(v.y, v.x)};
        
        ofRotateZ(angle);
        ofRotateX(ofRadToDeg(radian));
        ofTranslate(0.f, mRadius);
        
        ofColor c;
        if (i == (data.size()-1)) {
            c = color::kMain;
            ofSetLineWidth(4.f);
        }
        else {
            c = ofColor::white;
            ofSetLineWidth(2.f);
        }
        const float h{-v.length() * mRadius * 0.5f};
        ofFill();
        ofSetColor(c*0.5f, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofNoFill();
        ofSetColor(c, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofPopMatrix();
    }
}

#pragma mark ___________________________________________________________________
void SceneDataSphere::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
}

void SceneDataSphere::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataSphere::enter()
{
    dpDebugFunc();
    
    mCircles.assign(kNumCircles, ofPtr<Circle>());
    for (auto& p : mCircles) {
        p = ofPtr<Circle>(new Circle());
    }
}

void SceneDataSphere::exit()
{
    dpDebugFunc();
    mCircles.clear();
}

void SceneDataSphere::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        
        for (int i=0; i<kNumCircles ; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
            mCircles.at(i)->update(v);
        }
    }
}

void SceneDataSphere::draw()
{
    mCam.begin();
    ofPushMatrix();
    for (int i=0; i<mCircles.size(); i++) {
        ofPushMatrix();
        ofTranslate(getLineUped(kW, i, mCircles.size()), 0.f);
        mCircles.at(i)->draw();
        ofPopMatrix();
    }
    ofPopMatrix();
    mCam.end();
}

DP_SCORE_NAMESPACE_END