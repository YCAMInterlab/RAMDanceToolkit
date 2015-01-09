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
mName(""),
mId(-1)
{
}

void SceneBase::setId(int id)
{
    if (mId == -1)
        mId = id;
    else
        ofxThrowException(ofxException, "mId was already set by another process");
}

int SceneBase::getId() const
{
    return mId;
}

void SceneBase::setName(const string& name)
{
    if (mName == "")
        mName = name;
    else
        ofxThrowException(ofxException, "couldn't change name bacause it's already refered by another process");
}

const string& SceneBase::getName()
{
    if (mName == "")
        mName = getClassName(*this);
    return mName;
}

DP_SCORE_NAMESPACE_END