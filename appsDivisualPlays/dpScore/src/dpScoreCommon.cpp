//
//  dpScoreCommon.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreCommon.h"
#include <cxxabi.h>

const int kW = 1280;
const int kH = 768;

const int dpScoreFrameRate = 30;
const int dpScoreOscClientPort = 10000;

const string dpOscAddrCameraUnitPendulumVector = "/dp/cameraUnit/pendulum/vector";

const string dpScoreSettingsDir = "settings/";
const string dpScoreSettingsPrefix = "scoreUI-";

const string dpScoreMessageVec2 = "/dp/score/vec2";
const string dpScoreMessageVec2Arr = "/dp/score/vec2Arr";

struct _dpHandle {
    char* p;
    _dpHandle(char* ptr) : p(ptr) { }
    ~_dpHandle() { std::free(p); }
};

string dpDemangle(const char* name)
{    
    int status = -4;
    
    _dpHandle result(abi::__cxa_demangle(name, NULL, NULL, &status));
    
    return (status==0) ? result.p : name;
}

float dpAlign(float f)
{
    return ::floor(f) + 0.5f;
}

