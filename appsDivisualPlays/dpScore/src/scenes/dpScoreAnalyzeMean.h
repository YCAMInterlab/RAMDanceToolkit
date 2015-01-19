//
//  dpScoreAnalyzeMean.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/18/15.
//
//

#ifndef __dpScore__dpScoreAnalyzeMean__
#define __dpScore__dpScoreAnalyzeMean__

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class AnalyzeMean {
public:
    void update();
    
    ofVec4f mMean;
    ofVec4f mMeanPrev;
    ofVec4f mMeanAddtion;
    float mMeanLimit{5.f};
    int mPrevScene{0};
    float mPrevSetSceneTime{0.f};
    float mMinSetSceneTime{0.f};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreAnalyzeMean__) */
