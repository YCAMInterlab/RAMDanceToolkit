//
//  dpScoreSceneDataWave.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#include "dpScoreSceneDataWave.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneDataWave::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    mCam.setFarClip(10000);
}

void SceneDataWave::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataWave::enter()
{
    dpDebugFunc();
    
    mPlaneMesh = ofMesh::plane(kNumX * kStepX, kNumY * kStepY, kNumX, kNumY);
    mVbo.setVertexData(mPlaneMesh.getVerticesPointer(),
                       mPlaneMesh.getNumVertices(),
                       GL_DYNAMIC_DRAW);
    if (mPlaneMesh.hasIndices()) {
        mVbo.setIndexData(mPlaneMesh.getIndexPointer(),
                           mPlaneMesh.getNumIndices(),
                           GL_DYNAMIC_DRAW);
    }
    
    mData.assign(kNumX, deque<float>(kNumY));
    mVectors.assign(kNumCameraunitVectors, ofVec2f::zero());
    
    //mEnterTime = ofGetElapsedTimef();
    
    mFont.loadFont(kFontPath, 24.f);
}

void SceneDataWave::exit()
{
    dpDebugFunc();
    
    mData.clear();
    
    mVbo.clear();
    mPlaneMesh.clear();
    mVectors.clear();
    
    mFont = ofTrueTypeFont();
}

void SceneDataWave::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<mData.size(); i++) {
            if (i>=m.getNumArgs()) break;
            
            auto& deque = mData.at(i);
            const float f{clamp(m.getArgAsFloat(i))};
            deque.push_back(f);
            while (deque.size() > kNumY) {
                deque.pop_front();
            }
        }
        for (int i=0; i<mVectors.size(); i++) {
            if (i*2+0>=m.getNumArgs() || i*2+1>=m.getNumArgs()) break;
            mVectors.at(i).x = clamp(m.getArgAsFloat(i*2+0), 5.f);
            mVectors.at(i).y = clamp(m.getArgAsFloat(i*2+1), 5.f);
        }
    }
}

void SceneDataWave::draw()
{
    auto& v = mPlaneMesh.getVertices();
    for (int j=0; j<kNumY; j++) {
        for (int i=0; i<kNumX; i++) {
            const int index = j * kNumX + i;
            v.at(index).z = mData.at(i).at(j) * mScale;
        }
    }
    mVbo.updateVertexData(mPlaneMesh.getVerticesPointer(),
                          mPlaneMesh.getNumVertices());
    if (mPlaneMesh.hasIndices()) {
        mVbo.updateIndexData(mPlaneMesh.getIndexPointer(),
                             mPlaneMesh.getNumIndices());
    }
    
    const float time{ofGetElapsedTimef()-mEnterTime};
    mCam.begin();
    ofPushMatrix();
    ofRotateZ(time * 3.f);
    ofRotateY(time * 5.f);
    const GLuint mode{ofGetGLPrimitiveMode(mPlaneMesh.getMode())};
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ofSetColor(ofColor::white, 64);
    mVbo.getUsingIndices() ? mVbo.drawElements(mode, mVbo.getNumIndices()) : mVbo.draw(mode, 0, mVbo.getNumVertices());
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glPointSize(2.f);
    ofSetColor(ofColor::white, 256);
    mVbo.getUsingIndices() ? mVbo.drawElements(mode, mVbo.getNumIndices()) : mVbo.draw(mode, 0, mVbo.getNumVertices());
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ofPopMatrix();
    mCam.end();
    
    ofSetColor(180, 255);
    int i{0};
    const float h{mFont.getLineHeight()};
    for (auto& v : mVectors) {
        ofPushMatrix();
        ofTranslate(12.f,
                    (kH - mVectors.size() * h) + i * h + h * 0.5f,
                    0.0f);
        stringstream ss;
        ss << "v" << i << ": " << v;
        mFont.drawString(ss.str(), 0.f, 0.f);
        ofPopMatrix();
        i++;
    }
}

DP_SCORE_NAMESPACE_END