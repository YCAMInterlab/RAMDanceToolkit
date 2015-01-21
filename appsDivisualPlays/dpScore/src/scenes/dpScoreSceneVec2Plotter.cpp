//
//  dpScoreSceneVec2Plotter.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2Plotter.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneVec2Plotter::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 5.0f, &mSensorScale);
}

void SceneVec2Plotter::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneVec2Plotter::enter()
{
    dpDebugFunc();
    mCam.enableMouseInput();
    mCam.setFarClip(_circleNum * kStepZ);
    mEnterTime = ofGetElapsedTimef();
    
    mCircleBuffer.clear();
    mCircleVertices.clear();
    mCircleColors.clear();
    mCircleVertices.assign(_circleNum, ofVec3f::zero());
    mCircleColors.assign(_circleNum, ofFloatColor(0.f, 0.f, 0.f, 0.f));
    mCircleVbo.setVertexData(&mCircleVertices.at(0), _circleNum, GL_DYNAMIC_DRAW);
    mCircleVbo.setColorData(&mCircleColors.at(0), _circleNum, GL_DYNAMIC_DRAW);
}

void SceneVec2Plotter::exit()
{
    dpDebugFunc();
    mCam.disableMouseInput();
    
    mCircleBuffer.clear();
    mCircleVertices.clear();
    mCircleColors.clear();
    mCircleVbo.clear();
}

void SceneVec2Plotter::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mCircleBuffer.push_back(mVec);
        while (mCircleBuffer.size()>_circleNum) {
            mCircleBuffer.pop_front();
        }
    }
}

void SceneVec2Plotter::draw()
{
    const float halfH{(kH-50.f) * 0.5f};
    const float mult{halfH * mSensorScale};
    const float t{ofGetElapsedTimef()-mEnterTime};
    
    mCam.begin();
    ofRotateZ(t*2.f);
    ofRotateY(t*3.f);
    ofRotateX(t*1.f);
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        ofVec3f v{mCircleBuffer.at(i) * mult};
        const float xr{(kW-30.f)*0.5f};
        const float yr{(kH-60.f)*0.5f};
        v.x = ofClamp(v.x, -xr, xr);
        v.y = ofClamp(v.y, -yr+10.f, yr+10.f);
        v.z = (mCircleBuffer.size()-i)*-10.f;
        mCircleVertices.at(i) = v;
        float a{i / (float)mCircleBuffer.size()};
        a = easeOutExpo(a);
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a * 1.f);
    }
    
    mCircleVbo.updateVertexData(&mCircleVertices.at(0), mCircleVertices.size());
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_POINTS, 0, mCircleVertices.size());
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        float a{i / (float)mCircleBuffer.size()};
        a = easeOutExpo(a);
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a * 0.9f);
    }
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_LINE_STRIP, 0, mCircleVertices.size());
    
    const ofVec2f v{project(mVec * mult)};
    
    mCam.end();
    
    ofSetColor(ofColor::white, 200);
    ofNoFill();
    ofSetLineWidth(1.f);
    alignedRectangle(5.f, 25.f, kW-10.f, kH-30.f);
    
    const float step{(kW-10) / 382.f};
    for (int i=2; i<=380; i++) {
        int height{5};
        if (i%10==0) height = 8;
        alignedLine(5.f+i*step, 25.f, 5.f+i*step, 25.f+height);
        alignedLine(5.f+i*step, kH - 5.f, 5.f+i*step, kH - 5.f - height);
    }
    
    const float stepY{(kH - 30.f) / 175};
    for (int i=2; i<=173; i++) {
        int width{5};
        if (i%10 == 0) width = 8;
        alignedLine(5.f, 25.f+i*stepY, 5.f+width, 25.f+i*stepY);
        alignedLine(kW - 5.f, 25.f+i*stepY, kW - 5.f - width, 25.f+i*stepY);
    }
    
    alignedLine(v.x-5.f, 40.f, v.x-5.f, kH-20.f);
    alignedLine(20.f, v.y, kW-20.f, v.y);
    
    ofSetLineWidth(2.f);
    ofSetColor(color::kMain, 255);
    const float size{10.f};
    ofLine(v.x-5.f, v.y-size, v.x-5.f, v.y+size);
    ofLine(v.x-size-5.f, v.y, v.x+size-5.f, v.y);
}

DP_SCORE_NAMESPACE_END