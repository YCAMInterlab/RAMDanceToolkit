//
//  dpScoreSceneMasterIncrement.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreSceneMasterIncrement.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________
MasterIncrementNode::MasterIncrementNode()
{
}

MasterIncrementNode& MasterIncrementNode::operator = (const MasterIncrementNode& rhs)
{
    return *this = rhs;
}

#pragma mark ___________________________________________________________________
void SceneMasterIncrement::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
}

void SceneMasterIncrement::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneMasterIncrement::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mAdditions.assign(MH::NUM_VALVE_PINS, 0.f);
}

void SceneMasterIncrement::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneMasterIncrement::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        
    }
    if (m.getAddress() == kOscAddrCameraUnitMean) {
        if (m.getNumArgs() == 4) {
            ofVec4f mean;
            mean.x = m.getArgAsInt32(0);
            mean.y = m.getArgAsInt32(1);
            mean.z = m.getArgAsInt32(2);
            mean.w = m.getArgAsInt32(3);
            mMean.update(mean);
        }
    }
    if (m.getAddress() == kOscAddrCameraUnitVector) {
        for (int i=0; i<kNumCameraunitVectors ; i++) {
            if (2*i+0 >= m.getNumArgs() || 2*i+1 >= m.getNumArgs()) break;
            ofVec2f v(m.getArgAsFloat(2*i+0), m.getArgAsFloat(2*i+1));
        }
    }
    
    if (mPrevOfFrame == ofGetFrameNum()) return;
    
    mFrameNum++;
    mPrevOfFrame = ofGetFrameNum();
    
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        if (i<mAdditions.size()) {
            auto& a = mAdditions.at(i);
            a = 0.f;
            for (auto& n : skl->getJoints()) {
                n.update();
                a += n.totalDistance;
            }
            if (a >= mLimit) {
                getMH().turnOnValve(i);
                for (auto& n : skl->getJoints()) n.totalDistance = 0.f;
            }
        }
    }
    
    
}

void SceneMasterIncrement::draw()
{
    for (int i=0; i<getNumSkeletons(); i++) {
        auto skl = getSkeleton(i);
        
        mCam.begin();
        ofPushMatrix();
        ofNoFill();
        ofTranslate(getLineUped(kW, i, getNumSkeletons()) , -300.f, 0.f);
        ofxMot::drawSkeleton(skl);
        ofPopMatrix();
        mCam.end();
    }
    
    ofSetColor(ofColor::white);
    ofPushMatrix();
    alignedTranslate(0.f, 200.f);
    int i=0;
    for (auto f : mAdditions) {
        ofPushMatrix();
        ofTranslate(20.f, 20.f * i + 70.f);
        auto s = ofToString(f, 3) + "/" + ofToString(mLimit);
        ofDrawBitmapString(s, ofPoint::zero());
        ofPopMatrix();
        i++;
    }
    stringstream ss;
    ss << mMean.mMean << " / " << mMean.mMeanAddtion << endl;
    ss << kSceneNames[mMean.mPrevScene] << endl;
    ofDrawBitmapString(ss.str(), 20.f, 30.f);
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneMasterIncrement::setupSkeleton(SkeletonPtr skl)
{
}

void SceneMasterIncrement::updateSkeleton(SkeletonPtr skl)
{
}

void SceneMasterIncrement::exitSkeleton(SkeletonPtr skl)
{
}

DP_SCORE_NAMESPACE_END