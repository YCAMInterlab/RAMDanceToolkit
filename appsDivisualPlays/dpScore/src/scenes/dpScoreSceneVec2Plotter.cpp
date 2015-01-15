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
    
    mCircleBuffer.clear();
    
    mCircleVertices.clear();
    mCircleVertices.assign(_circleNum, ofVec3f::zero());
    mCircleColors.clear();
    mCircleColors.assign(_circleNum, ofFloatColor(0.f, 0.f, 0.f, 0.f));
    mCircleVbo.setVertexData(&mCircleVertices.at(0), _circleNum, GL_DYNAMIC_DRAW);
    mCircleVbo.setColorData(&mCircleColors.at(0), _circleNum, GL_DYNAMIC_DRAW);

    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
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
}

void SceneVec2Plotter::exit()
{
    dpDebugFunc();
}

void SceneVec2Plotter::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCaneraUnitVector) {
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
    ofPushStyle();
    ofSetLineWidth(1.f);
    ofPushMatrix();
    const float halfH{(kH-50.f) * 0.5f};
    const float mult{halfH * mSensorScale};
    ofVec2f v = mVec * mult;
    
    glPointSize(2.f);
    ofSetColor(255, 200);
    ofNoFill();
    alignedRectangle(5.f, 25.f, kW-10.f, kH-30.f);
    
    alignedLine(v.x+kW*0.5f-5.f, 40.f, v.x+kW*0.5f-5.f, kH-20.f);
    alignedLine(20.f, v.y+kH*0.5f, kW-20.f, v.y+kH*0.5f);
    
    ofSetColor(255, 200);
    ofSetLineWidth(1.f);
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
    
    ofSetColor(255, 255);
    alignedTranslate(kW * 0.5f, kH * 0.5f);
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        ofVec2f v{mCircleBuffer.at(i) * mult};
        const float xr{(kW-30.f)*0.5f};
        const float yr{(kH-60.f)*0.5f};
        v.x = ofClamp(v.x, -xr, xr);
        v.y = ofClamp(v.y, -yr+10.f, yr+10.f);
        mCircleVertices.at(i) = v;
        const float a{i / (float)mCircleBuffer.size()};
        //const float a = 1.f;
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a);
    }
    
    mCircleVbo.updateVertexData(&mCircleVertices.at(0), mCircleVertices.size());
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_POINTS, 0, mCircleVertices.size());
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        const float a{i / (float)mCircleBuffer.size()};
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a * 0.5f);
    }
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_LINE_STRIP, 0, mCircleVertices.size());
    
    ofPopMatrix();
    ofPopStyle();

    ofPushStyle();
    ofSetLineWidth(2.f);
    ofSetColor(color::kMain, 255);
    const float size{10.f};
    ofLine(v.x+kW*0.5f-5.f, v.y+kH*0.5f-size, v.x+kW*0.5f-5.f, v.y+kH*0.5f+size);
    ofLine(v.x+kW*0.5f-size-5.f, v.y+kH*0.5f, v.x+kW*0.5f+size-5.f, v.y+kH*0.5f);
    ofPopStyle();
}

DP_SCORE_NAMESPACE_END