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
    
    for (int i=0; i<mMeanAddtion.DIM; i++) {
        mMeanAddtion[i] += ::fabsf((mMean[i] - mMeanPrev[i])) * 0.01;
    }
    mMeanPrev = mMean;
    
    if (t < mPrevSetSceneTime + mMinSetSceneTime) return;
    
    const int dim{3};
    for (int i=0; i<dim; i++) {
        if (mMeanAddtion[i] >= mMeanLimit) {
            const int which  = (int)mMean[i] & 0b00000011;
            const bool scr0  = (int)mMean[i] & 0b00001000;
            const bool scr1  = (int)mMean[i] & 0b00010000;
            cout << i << ":" << which << ":" << scr0 << scr1 << endl;
            if (!scr0 && !scr1) continue;
            
            mMeanAddtion[i] = 0.f;
            const int scene{(int)ofClamp(which + i * 4, 0, MH::kNumScenes)};
            
            getMH().sendScene(MH::kSceneNames[scene], true, scr0, scr1);
            mPrevScene = scene;
            mPrevSetSceneTime = t;
        }
    }
}

DP_SCORE_NAMESPACE_END