//
//  dpScoreSceneVec2Plotter.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2Plotter.h"

DP_SCORE_NAMESPACE_BEGIN

static const int _circleNum = 2000;

void SceneVec2Plotter::initialize()
{
    dpDebugFunc();
    
    mSensorScale = 0.3f;
    
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
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void SceneVec2Plotter::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = NULL;
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
    if (m.getAddress() == kAddrVec2) {
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
    ofEnableAlphaBlending();
    
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString("Dividual Plays System Score - D", 12.f, 16.f);
    
    ofSetLineWidth(1.f);
    ofPushMatrix();
    const float mult = kH / 40.f * mSensorScale;
    ofVec2f v = mVec * mult;
    
    glPointSize(2.f);
    ofSetColor(255, 200);
    ofNoFill();
    ofRect(5.f, 25.f, kW-10.f, kH-30.f);
    
    ofLine(v.x+kW*0.5f, 40.f, v.x+kW*0.5f, kH-20.f);
    ofLine(20.f, v.y+kH*0.5f, kW-20.f, v.y+kH*0.5f);
    
    ofSetLineWidth(2.f);
    ofSetColor(255, 0, 0, 255);
    //ofLine(v.x+kW*0.5f, 40.f, v.x+kW*0.5f, 50.f);
    //ofLine(v.x+kW*0.5f, kH-30.f, v.x+kW*0.5f, kH-20.f);
    //ofLine(20.f, v.y+kH*0.5f, 30.f, v.y+kH*0.5f);
    //ofLine(kW-30.f, v.y+kH*0.5f, kW-20.f, v.y+kH*0.5f);
    
    const float size = 10.f;
    ofLine(v.x+kW*0.5f, v.y+kH*0.5f-size, v.x+kW*0.5f, v.y+kH*0.5f+size);
    ofLine(v.x+kW*0.5f-size, v.y+kH*0.5f, v.x+kW*0.5f+size, v.y+kH*0.5f);
    
    ofSetColor(255, 200);
    ofSetLineWidth(1.f);
    const float step = 1910 / 300;
    for (int i=2; i<=210; i++) {
        int height = 5;
        if (i%10==0) height = 8;
        ofLine(alignf(5.f+i*step), 25.f, alignf(5.f+i*step), 25.f+height);
        ofLine(alignf(5.f+i*step), kH - 5.f, alignf(5.f+i*step), kH - 5.f - height);
    }
    
    
    const float stepY = (kH - 30.f) / 100.f;
    for (int i=2; i<=98; i++) {
        int width = 5.f;
        if (i%10 == 0) width = 8;
        ofLine(5.f, alignf(25.f+i*stepY), 5.f+width, alignf(25.f+i*stepY));
        ofLine(kW - 5.f, alignf(25.f+i*stepY), kW - 5.f - width, alignf(25.f+i*stepY));
    }
    
    
    ofSetColor(255, 255);
    ofTranslate(kW * 0.5f, kH * 0.5f);
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        const float a = i / (float)mCircleBuffer.size();
        //const float a = 1.f;
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a);
    }
    
    mCircleVbo.updateVertexData(&mCircleVertices.at(0), mCircleVertices.size());
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_POINTS, 0, mCircleVertices.size());
    
    for (int i=mCircleBuffer.size()-1; i>=0; i--) {
        mCircleVertices.at(i) = mCircleBuffer.at(i) * mult;
        const float a = i / (float)mCircleBuffer.size();
        //const float a = 0.5f;
        mCircleColors.at(i).set(1.f, 1.f, 1.f, a * 0.5f);
    }
    mCircleVbo.updateColorData(&mCircleColors.at(0), mCircleColors.size());
    
    mCircleVbo.draw(GL_LINE_STRIP, 0, mCircleVertices.size());
    ofPopMatrix();
    ofPopStyle();

}

DP_SCORE_NAMESPACE_END