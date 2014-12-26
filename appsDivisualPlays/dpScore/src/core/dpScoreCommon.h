//
//  dpScoreCommon.h
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#ifndef __dpScore__dpScoreCommon__
#define __dpScore__dpScoreCommon__

#include "ofMain.h"
#include "ofxEvent.h"
#include "ofxException.h"

#define DP_SCORE_NAMESPACE_BEGIN namespace dp { namespace score {
 
#define DP_SCORE_NAMESPACE_END } }

DP_SCORE_NAMESPACE_BEGIN

extern const int kW;
extern const int kH;

extern const int kFrameRate;
extern const int kOscClientPort;

extern const string kOscAddrPendulumVec2;

extern const string kAddrVec2;
extern const string kAddrVec2Arr;

extern const string kSettingsDir;
extern const string kSettingsPrefix;

string demangle(const char* name);

template <class T>
string getClassName(const T& t)
{
    return demangle(typeid(t).name());
}

template <class T>
string getClassName()
{
    return demangle(typeid(T).name());
}

float alignf(float f);

DP_SCORE_NAMESPACE_END

#define dpDebugFunc() ofLogVerbose() << getClassName(*this) << ", " << __FUNCTION__

#endif /* defined(__dpScore__dpScoreCommon__) */
