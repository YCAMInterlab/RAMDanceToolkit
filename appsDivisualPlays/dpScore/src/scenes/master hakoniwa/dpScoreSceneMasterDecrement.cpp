//
//  dpScoreSceneMasterDecrement.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/15.
//
//

#include "dpScoreSceneMasterDecrement.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________
MasterDecrementNode::MasterDecrementNode()
{
}

MasterDecrementNode& MasterDecrementNode::operator = (const MasterDecrementNode& rhs)
{
    return *this = rhs;
}

#pragma mark ___________________________________________________________________
void SceneMasterDecrement::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    
    mCam.disableMouseInput();
}

void SceneMasterDecrement::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneMasterDecrement::enter()
{
    dpDebugFunc();
    
    mCam.enableMouseInput();
    
    mAdditions.assign(MH::kNumValvePins, 0.f);
}

void SceneMasterDecrement::exit()
{
    dpDebugFunc();
    
    mCam.disableMouseInput();
}

void SceneMasterDecrement::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        
    }
    if (m.getAddress() == kOscAddrCameraUnitMean) {

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

void SceneMasterDecrement::draw()
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
    alignedTranslate(0.f, 117.f);
    int i=0;
    for (auto f : mAdditions) {
        ofPushMatrix();
        ofTranslate(12.f, 12.f * i);
        stringstream ss;
        ss << boolalpha << getMH().getIsOpeningValve(i)
        << ": " << ofToString(f, 3) + "\n" + ofToString(mLimit);
        ofDrawBitmapString(ss.str(), ofPoint::zero());
        ofPopMatrix();
        i++;
    }
    ofPopMatrix();
}

#pragma mark ___________________________________________________________________
void SceneMasterDecrement::setupSkeleton(SkeletonPtr skl)
{
}

void SceneMasterDecrement::updateSkeleton(SkeletonPtr skl)
{
}

void SceneMasterDecrement::exitSkeleton(SkeletonPtr skl)
{
}

DP_SCORE_NAMESPACE_END