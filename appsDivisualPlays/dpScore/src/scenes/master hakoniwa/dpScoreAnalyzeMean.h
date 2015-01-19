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
#include "dpScoreMasterHakoniwa.h"
#include "dpScoreAnalyzeBase.h"

DP_SCORE_NAMESPACE_BEGIN

struct AnalyzeMean final : public AnalyzeBase {
    void update() override;
    
    ofVec4f mMean;
    ofVec4f mMeanPrev;
    ofVec4f mMeanAddtion;
    float mMeanLimit{5};
    int mPrevScene{0};
};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreAnalyzeMean__) */
