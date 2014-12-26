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

extern const int kW;
extern const int kH;

extern const int dpScoreFrameRate;
extern const int dpScoreOscClientPort;

extern const string dpOscAddrCameraUnitPendulumVector;

extern const string dpScoreSettingsDir;
extern const string dpScoreSettingsPrefix;

extern const string dpScoreMessageVec2;
extern const string dpScoreMessageVec2Arr;

string dpDemangle(const char* name);

template <class T>
string dpGetClassName(const T& t)
{
    return dpDemangle(typeid(t).name());
}

template <class T>
string dpGetClassName()
{
    return dpDemangle(typeid(T).name());
}

float dpAlign(float f);

#define dpDebugFunc() ofLogVerbose() << dpGetClassName(*this) << ", " << __FUNCTION__

#endif /* defined(__dpScore__dpScoreCommon__) */
