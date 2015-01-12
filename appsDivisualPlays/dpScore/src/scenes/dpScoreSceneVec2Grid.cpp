//
//  dpScoreSceneVec2Grid.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2Grid.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneVec2Grid::initialize()
{
    dpDebugFunc();
    
    mGridBuffer.clear();
    mGridBuffer.assign(mGridW/mGridStep, ofVec2f::zero());
    mGridVertices.clear();
    mGridVertices.assign(mGridW / mGridStep * mGridH /mGridStep, ofVec3f::zero());
    mGridVbo.setVertexData(&mGridVertices.at(0),
                           mGridVertices.size(),
                           GL_DYNAMIC_DRAW);
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void SceneVec2Grid::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneVec2Grid::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
}

void SceneVec2Grid::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneVec2Grid::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrVec2) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mGridBuffer.push_back(mVec);
        while (mGridBuffer.size()>kW/mGridStep) {
            mGridBuffer.pop_front();
        }
    }
}

void SceneVec2Grid::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    drawHeader();
    
    mCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()*0.5f, 150.f, 0.f);
    
    ofPushMatrix();
    ofTranslate(mGridW, 0.f);
    ofSetColor(255, 50);
    const float length = ofGetHeight()*2.f;
    ofLine(-length, 0.f, length, 0.f);
    ofLine(0.f, -length, 0.f, length);
    ofLine(0.f, 0.f, -length, 0.f, 0.f, length);
    
    const float size = 10.f;
    ofSetColor(ofColor::red, 100);
    ofLine(-size, 0.f, size, 0.f);
    ofSetColor(ofColor::green, 100);
    ofLine(0.f, -size, 0.f, size);
    ofSetColor(ofColor::blue, 100);
    ofLine(0.f, 0.f, -size, 0.f, 0.f, size);
    
    ofPopMatrix();
    
    ofSetColor(255, 255);
    ofNoFill();
    for (int j=0; j<mGridH/mGridStep; j++) {
        for (int i=0; i<mGridW/mGridStep; i++) {
            const int idx = j * mGridW/mGridStep + i;
            ofVec2f v = mGridBuffer.at(i);
            float f = ofMap(v.y, -25.f, 25.f, -1.f, 1.f);
            mGridVertices.at(idx).x = alignf(i*mGridStep);
            mGridVertices.at(idx).y = alignf(j*mGridStep) * f * mSensorScale;
            mGridVertices.at(idx).z = v.x * 15.f * mSensorScale;
        }
    }
    mGridVbo.updateVertexData(&mGridVertices.at(0), mGridVertices.size());
    
    mGridVbo.draw(GL_POINTS, 0, mGridVertices.size());
    ofPopMatrix();
    mCam.end();
    
    ofPopStyle();
}

DP_SCORE_NAMESPACE_END