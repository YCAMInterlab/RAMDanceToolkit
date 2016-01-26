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
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.0f, &mSensorScale);
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
    
    mGridBuffer.clear();
    mGridVertices.clear();
    mGridBuffer.assign(mGridW/mGridStep, ofVec2f::zero());
    mGridVertices.assign(mGridW / mGridStep * mGridH /mGridStep, ofVec3f::zero());
    mGridVbo.setVertexData(&mGridVertices.at(0),
                           mGridVertices.size(),
                           GL_DYNAMIC_DRAW);
    
    mEnterTime = ofGetElapsedTimef();
}

void SceneVec2Grid::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
    
    mGridBuffer.clear();
    mGridVertices.clear();
    mGridVbo.clear();
}

void SceneVec2Grid::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVectorTotal) {
        mVec.x = m.getArgAsFloat(0);
        mVec.y = m.getArgAsFloat(1);
        mGridBuffer.push_back(mVec);
        while (mGridBuffer.size()>mGridW/mGridStep) {
            mGridBuffer.pop_front();
        }
    }
}

void SceneVec2Grid::draw()
{    
    mCam.begin();
    ofPushMatrix();
    const float t{::fmodf(ofGetElapsedTimef() - mEnterTime, 36.f)};
    
    if (t < 12.f) {
        ofTranslate(-ofGetWidth()*0.7f, -150.f, 0.f);
        ofRotateX(-30.f);
        ofRotateY(15.f);
        ofRotateZ(25.f);
    }
    else if (t < 24.f) {
        ofTranslate(-ofGetWidth()*0.7f, -100.f, -100.f);
        ofRotateX(90.f);
    }
    else {
        ofTranslate(-ofGetWidth()*0.7f, -100.f, -100.f);
    }
    
    ofPushMatrix();
    ofTranslate(mGridW, 0.f);
    ofSetColor(255, 128);
    const float length{ofGetHeight()*3.5f};
    glBegin(GL_LINES);
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f(-length, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f( length, 0.f, 0.f);
    
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f(0.f, -length, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f(0.f,  length, 0.f);
    
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f(0.f, 0.f, -length);
    glColor4f(1.f, 1.f, 1.f, 0.5f); glVertex3f(0.f, 0.f, 0.f);
    glColor4f(1.f, 1.f, 1.f, 0.0f); glVertex3f(0.f, 0.f,  length);
    glEnd();
    
    ofPushStyle();
    ofPushMatrix();
    ofVec3f plotter;
    plotter.x = 0.f;
    plotter.y = mVec.y * 0.3f * mSensorScale;
    plotter.z = mVec.x * 300.f * mSensorScale;
    ofTranslate(plotter);
    
    const float size{100.f};
    ofSetLineWidth(2.f);
    ofSetColor(color::kMain, 255);
    ofLine(0.f, -size, 0.f, size);
    ofPopStyle();
    ofPopMatrix();
    
    ofPopMatrix();
    
    ofSetColor(255, 255);
    ofNoFill();
    for (int j=0; j<mGridH/mGridStep; j++) {
        for (int i=0; i<mGridW/mGridStep; i++) {
            const int idx{j * mGridW/mGridStep + i};
            ofVec2f v{mGridBuffer.at(i)};
            mGridVertices.at(idx).x = i*mGridStep;
            mGridVertices.at(idx).y = j*mGridStep * v.y * 0.3f * mSensorScale;
            mGridVertices.at(idx).z = v.x * 300.f * mSensorScale;
        }
    }
    mGridVbo.updateVertexData(&mGridVertices.at(0), mGridVertices.size());
    
    mGridVbo.draw(GL_POINTS, 0, mGridVertices.size());
    ofPopMatrix();
    mCam.end();
}

DP_SCORE_NAMESPACE_END