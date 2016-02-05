//
//  dpScoreUtils.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#ifndef dpScoreUtils_h
#define dpScoreUtils_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

float getLineUped(float length,
                  int index,
                  int total,
                  bool fromCenter = true);

float clamp(float f, float range = 1.f);

ofVec3f randVec3f();

ofVec3f project(const ofVec3f& obj);
ofVec3f unproject(const ofVec2f& screen);

void billboard();

float getElapsedTime();
void updateElapsedTime();
void setPauseElapsedTimeCounter(bool pause);

extern const string kEventAddrChangeScene;

template<class SceneClass>
void notifyChangeScene()
{
    ofxEventMessage m;
    m.setAddress(kEventAddrChangeScene);
    const string className = getClassName<SceneClass>();
    m.addStringArg(className);
    ofxNotifyEvent(m);
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreUtils_h */
