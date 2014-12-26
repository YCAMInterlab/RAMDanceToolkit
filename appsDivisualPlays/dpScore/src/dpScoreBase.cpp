//
//  dpScoreBase.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreBase.h"

dpScoreBase::dpScoreBase() :
mUICanvas(NULL),
mName("")
{
}

const string& dpScoreBase::getName()
{
    if (mName == "")
        mName = dpGetClassName(*this);
    return mName;
}