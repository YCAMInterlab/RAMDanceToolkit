//
//  dpScoreSceneBodyPatterns.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/10/15.
//
//

#include "dpScoreSceneBodyPatterns.h"

DP_SCORE_NAMESPACE_BEGIN


void SceneBodyPatterns::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
    
    mCam.setDistance(200);
    mCam.disableMouseInput();
}

void SceneBodyPatterns::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneBodyPatterns::enter()
{
    dpDebugFunc();
    
    ofAddListener(ofxMotioner::updateSkeletonEvent,
                  this,
                  &SceneBodyPatterns::onUpdateSkeleton);
    mCam.enableMouseInput();
    
    mSkeletons.clear();
    mSkeletonName = "";
    mRange.clear();
    mRange.push_back(Range(ofxMot::JOINT_HIPS, ofxMot::JOINT_HEAD));
    mRange.push_back(Range(ofxMot::JOINT_LEFT_HIP, ofxMot::JOINT_LEFT_TOE));
    mRange.push_back(Range(ofxMot::JOINT_RIGHT_HIP, ofxMot::JOINT_RIGHT_TOE));
    mRange.push_back(Range(ofxMot::JOINT_LEFT_COLLAR, ofxMot::JOINT_LEFT_HAND));
    mRange.push_back(Range(ofxMot::JOINT_RIGHT_COLLAR, ofxMot::JOINT_RIGHT_HAND));
}

void SceneBodyPatterns::exit()
{
    dpDebugFunc();
    
    ofRemoveListener(ofxMotioner::updateSkeletonEvent,
                     this,
                     &SceneBodyPatterns::onUpdateSkeleton);
    mCam.disableMouseInput();
    
    mSkeletons.clear();
}

void SceneBodyPatterns::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrMotioner) {
        mFrame++;
        if (mFrame%60 == 0)  {
            (++mFocusNode) %= ofxMot::NUM_JOINTS;
        }
    }
}

void SceneBodyPatterns::draw()
{
    if (mFrame % (60 * 6) == 0) {
        ++mCurrentRange %= mRange.size();
    }
    
    const float stepX = aligned(kW/nX);
    const float stepY = aligned((kH-20.f)/nY);
    for (int j=0; j<nY; j++) {
        for (int i=0; i<nX; i++) {
            mViewport = alignedRectangle(i*stepX, j*stepY+20.f, stepX, stepY);
            const int idx = mSkeletons.size() - (i + j * nX) - 1;
            if (idx<mSkeletons.size()) {
                drawSkeleton(mSkeletons.at(idx), i + j * nX);
            }
            
            ofNoFill();
            ofSetLineWidth(1.f);
            ofSetColor(ofColor::white);
            ofRect(mViewport);
        }
    }
    alignedLine(kW-1, 20.f, kW-1, kH);
    alignedLine(0.f, kH-1.f, kW, kH-1.f);
}

void SceneBodyPatterns::drawSkeleton(ofxMot::SkeletonPtr skl, int idx)
{
    auto& joints = skl->getJoints();
    
    ofNoFill();
    ofSetLineWidth(1.5f);
    
    auto r = mRange.at(mCurrentRange);
    const int size{r.end - r.begin};
    
    ofVec3f centroid;
    for (size_t i=r.begin; i<r.end; i++) {
        auto& n = skl->getJoint(i);
        centroid += n.getGlobalPosition();
    }
    centroid /= size;
    
    mCam.begin(mViewport);
    ofPushMatrix();
    ofScale(mScale, mScale, mScale);
    ofTranslate(-centroid.x, -centroid.y, -centroid.z);
    vector<ofVec3f> points(size);
    for (size_t i=r.begin; i<r.end; i++) {
        auto& n = skl->getJoint(i);
        idx == 0 ? ofSetColor(color::kMain) : ofSetColor(ofColor::white);
        n.transformGL();
        ofDrawBox(10.f);
        n.restoreTransformGL();
        if (i != r.begin && n.getParent()) {
            ofSetColor(ofColor::white);
            ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
        }
    }
    ofPopMatrix();
    mCam.end();
}

#pragma mark ___________________________________________________________________
void SceneBodyPatterns::onUpdateSkeleton(ofxMotioner::EventArgs &e)
{
    auto skl = e.skeleton;
    skl->getJoint(ofxMot::JOINT_HIPS).setGlobalPosition(ofVec3f::zero());
    
    if (mSkeletonName=="") mSkeletonName = skl->getName();
    
    if (mSkeletonName == skl->getName()) {
        if (mFrame % kSkip == 0) {
            auto copy = ofxMot::Skeleton::copy(skl);
            
            mSkeletons.push_back(copy);
            
            while (mSkeletons.size() > kNumSkeletons) {
                mSkeletons.pop_front();
            }
        }
        else if (mSkeletons.empty() == false) {
            *(mSkeletons.end()-1) = ofxMot::Skeleton::copy(skl);
        }
    }
}

DP_SCORE_NAMESPACE_END