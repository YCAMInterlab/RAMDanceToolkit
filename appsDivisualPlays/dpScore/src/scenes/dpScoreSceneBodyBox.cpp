//
//  dpScoreSceneBodyBox.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/14/15.
//
//

#include "dpScoreSceneBodyBox.h"

DP_SCORE_NAMESPACE_BEGIN

BodyBoxNode::BodyBoxNode()
{
    screenCoords.clear();
    randomPoints.assign(kMaxDivision, vector<ofVec3f>(kNumRandomPoints));
    for_each(randomPoints.begin(), randomPoints.end(), [](vector<ofVec3f>& vec)
             {
                 for_each(vec.begin(), vec.end(), [](ofVec3f& p)
                          {
                              const ofVec3f r = randVec3f();
                              p = r * 20.f;
                          });
             });
}

BodyBoxNode& BodyBoxNode::operator = (const BodyBoxNode& rhs)
{
    return *this = rhs;
}

void BodyBoxNode::update()
{
    screenCoords.clear();
    
    if (getParent()) {
        const ofVec3f begin = getGlobalPosition();
        const ofVec3f end = getParent()->getGlobalPosition();
        
        int div = begin.distance(end) / 15;
        div = ofClamp(div, 1.f, kMaxDivision-1.f);
        screenCoords.assign(div, ofVec3f::zero());
        
        for (int i=0; i<div; i++) {
            const float p = i / (float)(div-1);
            const ofVec3f v = begin.interpolated(end, p);
            screenCoords.at(i) = v;
            
            for_each(randomPoints.at(i).begin(), randomPoints.at(i).end(), [&](const ofVec3f& rp){
                screenCoords.push_back(v + rp);
            });
        }
    }
    else {
        screenCoords.push_back(getGlobalPosition());
    }
}

#pragma mark ___________________________________________________________________
void SceneBodyBox::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addSlider("Division", 4.f, 40.f, &mDiv);
    ofAddListener(mUICanvas->newGUIEvent, this, &SceneBodyBox::guiEvent);
    
    mCam.disableMouseInput();
    mCam.setFarClip(6000.f);
}

void SceneBodyBox::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyBox::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mGridFbo = ofPtr<ofFbo>(new ofFbo());
    mGridFbo->allocate(kW, kH, GL_RGBA);
    mFboReady = false;
    
    mLights.assign(kNumLights, ofPtr<ofLight>());
    for (auto& p : mLights) {
        p = ofPtr<ofLight>(new ofLight());
    }
}

void SceneBodyBox::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
    
    mLights.clear();
    
    mGridFbo = ofPtr<ofFbo>();
    mFboReady = false;
}

void SceneBodyBox::update(ofxEventMessage& m)
{
    if (m.getAddress() == kAddrMotioner) {
        for (int i=0; i<getNumSkeletons(); i++) {
            auto skl = getSkeleton(i);
            for (auto& n : skl->getJoints()) {
                n.setPosition(n.getPosition()*4.5f);
                n.update();
            }
        }
    }
}

void SceneBodyBox::updateLights()
{
    mLights.at(0)->setAmbientColor(0.1f);
//    mLights.at(0)->setDiffuseColor(0.5f);
//    mLights.at(0)->setSpecularColor(0.5f);
    mLights.at(0)->setDiffuseColor(1.f);
    mLights.at(0)->setSpecularColor(1.f);
    
    //mLights.at(1)->setAmbientColor(color::kDarkPinkHeavy);
    //mLights.at(1)->setDiffuseColor(color::kDarkPinkLight);
    //mLights.at(1)->setSpecularColor(color::kDarkPinkLight);
    
    //mLights.at(2)->setAmbientColor(color::kPalePinkHeavy);
    //mLights.at(2)->setDiffuseColor(color::kPalePinkLight);
    //mLights.at(2)->setSpecularColor(color::kPalePinkLight);
    
    mLights.at(0)->setPosition(ofVec3f(-300.f, 600.f, 400.f));
    
//    ofVec3f p1;
//    p1.x = ::cos(ofGetElapsedTimef() * 1.f) * 800.f;
//    p1.y = ::sin(ofGetElapsedTimef() * 2.f) * 600.f;
//    p1.z = ::cos(ofGetElapsedTimef() * 3.f) * 400.f;
//    mLights.at(1)->setPosition(p1);
    
    //ofVec3f p2;
    //p2.x = ::cos(ofGetElapsedTimef() * 3.f) * 300.f;
    //p2.y = ::sin(ofGetElapsedTimef() * 1.f) * 900.f;
    //p2.z = ::cos(ofGetElapsedTimef() * 2.f) * 500.f;
    //mLights.at(2)->setPosition(p2);
}

void SceneBodyBox::draw()
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
    //mGridFbo->draw(aligned(0.f), aligned(0.f));
    
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    
    updateLights();
    
    mCam.begin();
    ofRotateY(ofGetElapsedTimef()*10.f);
    
    //for (auto& l : mLights) {
    //    ofSetColor(l->getDiffuseColor());
    //    ofDrawSphere(l->getGlobalPosition(), 100.f);
    //}
    
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        ofPushMatrix();
        const int n = getNumSkeletons();
        const float step = kW/n;
        alignedTranslate(-kW*0.5f + step * 0.5f + step * i , -300.f, 0.f);
        
        for (auto& n : skl->getJoints()) {
            auto drawRect = [&](const ofVec3f& v) {
                int x = (int)v.x;
                int y = (int)v.y;
                int z = (int)v.z;
                
                x = (x / kDiv) * kDiv;
                y = (y / kDiv) * kDiv;
                z = (z / kDiv) * kDiv;
                
                ofEnableLighting();
                ofFill();
                
                for (auto l : mLights) l->enable();
                ofSetColor(ofColor::white, 255);
                ofDrawBox(alignedVec3f(x, y, z), aligned(kDiv));
                
                for (auto l : mLights) l->disable();
                
                ofDisableLighting();
                ofNoFill();
                
                ofSetLineWidth(2.f);
                ofSetColor(ofColor::black, 255);
                ofDrawBox(alignedVec3f(x, y, z), aligned(kDiv));
                
            };
            for_each(n.screenCoords.begin(), n.screenCoords.end(), drawRect);
        }
        
        ofPopMatrix();
    }
    
    mCam.end();
    
    ofFill();
    ofSetColor(ofColor::black, 255);
    alignedRect(0.f, 0.f, kW, 20.f);
}

#pragma mark ___________________________________________________________________
void SceneBodyBox::setupSkeleton(SkeletonPtr skl)
{
    for (auto& n : skl->getJoints()) {
        n.owner = this;
    }
}

void SceneBodyBox::updateSkeleton(SkeletonPtr skl)
{
    
}

void SceneBodyBox::exitSkeleton(SkeletonPtr skl)
{
    
}

#pragma mark ___________________________________________________________________
void SceneBodyBox::guiEvent(ofxUIEventArgs &e)
{
    if (e.widget->getName() == "Division") {
        mFboReady = false;
    }
}

DP_SCORE_NAMESPACE_END