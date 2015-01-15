//
//  dpScoreSceneBodyRect.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#include "dpScoreSceneBodyRect.h"

DP_SCORE_NAMESPACE_BEGIN

BodyRectNode::BodyRectNode()
{
    screenCoords.clear();
    randomPoints.assign(kMaxDivision, vector<ofVec2f>(kNumRandomPoints));
    for_each(randomPoints.begin(), randomPoints.end(), [](vector<ofVec2f>& vec)
             {
                 for_each(vec.begin(), vec.end(), [](ofVec2f& p)
                          {
                              const ofVec3f r = randVec3f();
                              const float s = ofRandom(10.f, 20.f);
                              p = r * s;
                          });
             });
}

BodyRectNode& BodyRectNode::operator = (const BodyRectNode& rhs)
{
    return *this = rhs;
}

void BodyRectNode::update()
{
    screenCoords.clear();
    transformGL();
    const ofVec2f screenCoord = project(ofVec3f::zero());
    restoreTransformGL();
    
    if (getParent()) {
        const ofVec3f begin = screenCoord;
        getParent()->transformGL();
        const ofVec3f end = project(ofVec3f::zero());
        getParent()->restoreTransformGL();
        
        int div = begin.distance(end) / 5;
        div = ofClamp(div, 1.f, kMaxDivision-1.f);
        screenCoords.assign(div, ofVec3f::zero());
        
        for (int i=0; i<div; i++) {
            const float p = i / (float)(div-1);
            const ofVec3f v = begin.interpolated(end, p);
            screenCoords.at(i) = v;
            
            for_each(randomPoints.at(i).begin(), randomPoints.at(i).end(), [&](const ofVec2f& rp){
                screenCoords.push_back(v + rp);
            });
        }
    }
    else {
        screenCoords.push_back(screenCoord);
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyRect::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Division", 4.f, 40.f, &mDiv);
    ofAddListener(mUICanvas->newGUIEvent, this, &SceneBodyRect::guiEvent);
    
    mCam.disableMouseInput();
}

void SceneBodyRect::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyRect::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mGridFbo = ofPtr<ofFbo>(new ofFbo());
    mGridFbo->allocate(kW, kH, GL_RGBA);
    mFboReady = false;
}

void SceneBodyRect::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
    
    mGridFbo = ofPtr<ofFbo>();
    mFboReady = false;
}

void SceneBodyRect::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        for (int i=0; i<getNumSkeletons(); i++) {
            auto skl = getSkeleton(i);
            mCam.begin();
            ofPushMatrix();
            ofRotateY(ofGetElapsedTimef()*50.f);
            const int n = getNumSkeletons();
            const float step = kW/n;
            ofTranslate(-kW*0.5f + step * 0.5f + step * i , -300.f, 0.f);
            for (auto& n : skl->getJoints()) {
                n.setPosition(n.getPosition()*4.5f);
                n.update();
            }
            ofPopMatrix();
            mCam.end();
        }
    }
}

void SceneBodyRect::draw()
{
    ofSetLineWidth(1.f);
    
    const int kDiv = (int)mDiv;
    
    if (mFboReady == false) {
        mGridFbo->begin();
        ofBackground(ofColor::black);
        ofNoFill();
        ofSetColor(128, 255);
        alignedLine(0.f, 21.f, kW, 21.f);
        alignedLine(1.f, 0.f, 1.f, kH);
        for (int i=0; i<kW/kDiv; i++) {
            for (int j=0; j<kH/kDiv; j++) {
                ofPushMatrix();
                alignedTranslate(i*kDiv, j*kDiv);
                alignedRect(0.f, 0.f, kDiv, kDiv);
                ofPopMatrix();
            }
        }
        mGridFbo->end();
        mFboReady = true;
    }
    mGridFbo->draw(aligned(0.f), aligned(0.f));
    
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        for (auto& n : skl->getJoints()) {
            auto drawRect = [&](const ofVec2f& v) {
                int x = (int)v.x;
                int y = (int)v.y;
                
                x = (x / kDiv) * kDiv;
                y = (y / kDiv) * kDiv;
                
                ofFill();
                ofSetColor(ofColor::white, 255);
                alignedRect(x, y, kDiv-1.f, kDiv-1.f);
                ofNoFill();
                //ofSetColor(ofColor::black, 255);
                ofSetColor(color::kMain, 255);
                alignedRect(x, y, kDiv, kDiv);
            };
            for_each(n.screenCoords.begin(), n.screenCoords.end(), drawRect);
        }
    }
    
    ofFill();
    ofSetColor(ofColor::black, 255);
    alignedRect(0.f, 0.f, kW, 20.f);
}

#pragma mark ___________________________________________________________________
void SceneBodyRect::setupSkeleton(SkeletonPtr skl)
{
    for (auto& n : skl->getJoints()) {
        n.owner = this;
    }
}

void SceneBodyRect::updateSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyRect::exitSkeleton(SkeletonPtr skl)
{
    
}

#pragma mark ___________________________________________________________________
void SceneBodyRect::guiEvent(ofxUIEventArgs &e)
{
    if (e.widget->getName() == "Division") {
        mFboReady = false;
    }
}

DP_SCORE_NAMESPACE_END