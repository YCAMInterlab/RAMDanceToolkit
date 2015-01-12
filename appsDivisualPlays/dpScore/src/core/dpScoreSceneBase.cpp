//
//  dpScoreSceneBase.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneBase.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneBase::drawHeader()
{
    const string title = "Dividual Plays Score";
    string name = getName();
    ofStringReplace(name, "dp::score::Scene", "");
    ofPushStyle();
    ofSetColor(color::kMain, 255);
    ofDrawBitmapString(title, 12.f, 16.f);
    ofSetColor(ofColor::white, 255);
    ofDrawBitmapString(name, 12.f + (title.length()-1) * 9.f, 16.f);
    ofPopStyle();
}

void SceneBase::setId(int id)
{
    if (mId == -1)
        mId = id;
    else
        ofxThrowException(ofxException, "mId was already set by another process");
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