//
//  dpScoreCompound.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompound.h"

DP_SCORE_NAMESPACE_BEGIN

static bool _enableCollapse = false;

void compoundSetEnableCollapse(bool enable)
{
	_enableCollapse = enable;
}

bool compoundGetEnableCollapse()
{
	return _enableCollapse;
}

DP_SCORE_NAMESPACE_END