//
//  dpScoreAnalyzeMean.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#include "dpScoreAnalyzeMean.h"

DP_SCORE_NAMESPACE_BEGIN

void AnalyzeMean::update(const ofVec4f& mean)
{
    mMean = mean;
    
    mMeanAddtion.x += ::fabsf((mMean.x - mMeanPrev.x)) * 0.01;
    mMeanAddtion.y += ::fabsf((mMean.y - mMeanPrev.y)) * 0.01;
    mMeanAddtion.z += ::fabsf((mMean.z - mMeanPrev.z)) * 0.01;
    mMeanAddtion.w += ::fabsf((mMean.w - mMeanPrev.w)) * 0.01;
    
    mMeanPrev = mMean;
    
    if (mMeanAddtion.x >= mMeanLimit) {
        mMeanAddtion.x = 0.f;
        const int which = ofRandom(2);
        const int scene = ofClamp(which * 3 + 0, 0, kNumScenes);
        getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
        getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
        mPrevScene = scene;
    }
    if (mMeanAddtion.y >= mMeanLimit) {
        mMeanAddtion.y = 0.f;
        const int which = ofRandom(2);
        const int scene = ofClamp(which * 3 + 1, 0, kNumScenes);
        getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
        getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
        mPrevScene = scene;
    }
    if (mMeanAddtion.z >= mMeanLimit) {
        mMeanAddtion.z = 0.f;
        const int which = ofRandom(2);
        const int scene = ofClamp(which * 3 + 2, 0, kNumScenes);
        getMH().sendScene(kSceneNames[scene], true, true, true, true, true);
        getMH().sendScene(kSceneNames[mPrevScene], false, true, true, true, true);
        mPrevScene = scene;
    }
}

DP_SCORE_NAMESPACE_END