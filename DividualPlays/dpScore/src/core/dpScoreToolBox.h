//
//  dpScoreToolBox.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreToolBox_h
#define dpScoreToolBox_h

#include "dpScoreAnyDraw.h"
#include "dpScoreMultiFrags.h"
#include "dpScoreLoop.h"

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

// no r-value references
template <typename T>
inline shared_ptr<T> makeShared()
{
    return shared_ptr<T>(new T());
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreToolBox_h */
