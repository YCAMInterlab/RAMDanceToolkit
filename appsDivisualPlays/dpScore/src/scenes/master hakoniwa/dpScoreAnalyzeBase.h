//
//  dpScoreAnalyzeBase.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/15.
//
//

#ifndef dpScore_dpScoreAnalyzeBase_h
#define dpScore_dpScoreAnalyzeBase_h

#include "dpScoreCommon.h"
#include "dpScoreMasterHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

struct AnalyzeBase {
    virtual void update() = 0;
};

DP_SCORE_NAMESPACE_END

#endif
