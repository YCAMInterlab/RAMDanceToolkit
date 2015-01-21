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

void AnalyzeMean::update(ofVec4f mean)
{
    mMean = mean;
    
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
    
    if (mTotalAddition.f >= mMeanLimit) {
        const bool win0  = (mTotalAddition.i & 0b00000001) >> 0;
        const bool win1  = (mTotalAddition.i & 0b00000010) >> 1;
        const int which  = (mTotalAddition.i & 0b00111100) >> 2;
        cout << which << ":" << win0 << win1 << endl;
        
        if (getMH().getUniqueScenes().empty() == false) {
            int scene{(int)(which % getMH().getUniqueScenes().size())};
            getMH().setUniqueScene(scene, win0, win1);
            mPrevScene = scene;
            mPrevSetSceneTime = t;
        }
        
        for (int i=0; i<mMeanAddtion.DIM; i++) {
            mMeanAddtion[i] = 0.f;
        }
    }
}

void AnalyzeMean::draw()
{
    ofPushStyle();
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
    
    ofPopStyle();
}

DP_SCORE_NAMESPACE_END