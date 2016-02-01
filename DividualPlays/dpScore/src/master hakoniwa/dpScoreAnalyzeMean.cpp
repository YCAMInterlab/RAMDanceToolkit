//
//  dpScoreAnalyzeMean.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreAnalyzeMean.h"
#include "dpScoreMasterHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

void AnalyzeMean::update()
{    
    const float t{ofGetElapsedTimef()};
    mLastUpdateSpan = t - mLastFrameTime;
    mLastFrameTime = t;
    
    mTotalAddition.f = 0.f;
    
    for (int i=0; i<mMeanAddtion.DIM; i++) {
        mMeanAddtion[i] += ::fabsf((mMean[i] - mMeanPrev[i])) * 0.01;
        mTotalAddition.f += mMeanAddtion[i];
    }
    mMeanPrev = mMean;
    
    if (t < mPrevSetSceneTime + mMinSetSceneTime) return;
    
    if (mMaster) {
        if (mTotalAddition.f >= mMeanLimit) {
            mWin0  = (mTotalAddition.i & 0b00000001) >> 0;
            mWin1  = (mTotalAddition.i & 0b00000010) >> 1;
            mWhich  = (mTotalAddition.i & 0b00111100) >> 2;
            
            if (getMH().getSceneController().getUniqueScenes().empty() == false) {
                const int scene{(int)(mWhich % getMH().getSceneController().getUniqueScenes().size())};
                //const int score{(int)(mWhich % getMH().getSceneController().getNumUniqueScores())};
                getMH().getSceneController().setUniqueScene(scene, mWin0, mWin1);
                //getMH().getSceneController().setUniqueScore(score);
                mPrevScene = scene;
                mPrevSetSceneTime = t;
            }
            
            for (int i=0; i<mMeanAddtion.DIM; i++) {
                mMeanAddtion[i] = 0.f;
            }
        }
    }
}

void AnalyzeMean::draw()
{
    const float t{ofGetElapsedTimef()};
    
    ofPushStyle();
    ofPushMatrix();
    alignedTranslate(0.f, MH::kTextSpacing);
    const float w{188.f};
    const float h{10.f};
    const float spacing{2.f};
    const int a{100};
    const ofColor colors[ofVec4f::DIM]{
        ofColor(255, 150, 150, a),
        ofColor(150, 255, 150, a),
        ofColor(150, 150, 255, a),
        ofColor(ofColor::white, a),
    };
    
    for (int i=0;i <mMean.DIM; i++) {
        ofSetColor(colors[i]);
        const float x{mMean[i]/255.f};
        alignedRect(0.f, h * i + i * spacing, w * x, h);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(MH::kTextColor);
    ofDrawBitmapString("[mean]", ofPoint::zero());
    alignedTranslate(200.f, MH::kTextSpacing);
    alignedTranslate(0.f, MH::kTextSpacing);
    stringstream ss;
    ss << fixed
    << "master    : " << mMaster << endl
    << "data span : " << setprecision(3) << mLastUpdateSpan << endl
    << "scene span: " << setprecision(1) << t - mPrevSetSceneTime << endl
    << "total add : " << setprecision(3) << mTotalAddition.f << endl
    << "raw color : " << setprecision(0) << mMean << endl
    << "diff add  : " << setprecision(1) << mMeanAddtion << endl
    << "gen       : " << mWhich << ", " << mWin0 << ", " << mWin1;
    
    ofDrawBitmapString(ss.str(), ofPoint::zero());
    ofPopMatrix();
    
    ofPopStyle();
}

DP_SCORE_NAMESPACE_END