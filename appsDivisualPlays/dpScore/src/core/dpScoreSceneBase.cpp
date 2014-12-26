//
//  dpScoreSceneBase.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

SceneBase::SceneBase() :
mUICanvas(NULL),
mName("")
{
}

void SceneBase::setName(const string& name)
{
    mName = name;
}

const string& SceneBase::getName()
{
    if (mName == "")
        mName = getClassName(*this);
    return mName;
}

DP_SCORE_NAMESPACE_END