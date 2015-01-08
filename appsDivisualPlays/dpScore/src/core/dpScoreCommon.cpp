//
//  dpScoreCommon.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreCommon.h"
#include <cxxabi.h>

DP_SCORE_NAMESPACE_BEGIN

const int kW = 1280;
const int kH = 768;

const int kFrameRate = 30;
const int kOscClientPort = 10000;

const string kOscAddrPendulumVec2 = "/dp/cameraUnit/pendulum/vector";

const string kAddrVec2 = "/dp/score/vec2";
const string kAddrVec2Arr = "/dp/score/vec2Arr";

const string kSettingsDir = "settings/";
const string kSettingsPrefix = "scoreUI-";

struct _handle {
    char* p;
    _handle(char* ptr) : p(ptr) {}
    ~_handle() { std::free(p); }
};

string demangle(const char* name)
{    
    int status = -4;
    
    _handle result(abi::__cxa_demangle(name, NULL, NULL, &status));
    
    return (status==0) ? result.p : name;
}

float alignf(float f)
{
    return ::floor(f) + 0.5f;
}

DP_SCORE_NAMESPACE_END