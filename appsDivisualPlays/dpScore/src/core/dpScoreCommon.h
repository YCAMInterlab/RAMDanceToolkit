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
#include "ofxMotioner.h"

// WIP
//#define USE_CUSTOM_MEMORY_ALLOCATOR

//#ifdef USE_CUSTOM_MEMORY_ALLOCATOR
//void* operator new(size_t size) throw(std::bad_alloc);
//void* operator new[](size_t size) throw(std::bad_alloc);
//void operator delete(void* pv) throw();
//void operator delete[](void* pv) throw();
//#endif

#define DP_SCORE_NAMESPACE_BEGIN namespace dp { namespace score {
#define DP_SCORE_NAMESPACE_END } }

DP_SCORE_NAMESPACE_BEGIN
using easeFunc = float(*)(float);

extern const int kW;
extern const int kH;

extern const int kFrameRate;
extern const int kOscClientPort;

extern const string kOscAddrChangeScene;
extern const string kOscAddrPendulumVec2;

extern const string kAddrVec2;
extern const string kAddrVec2Arr;
extern const string kAddrMotioner;

extern const string kSettingsDir;
extern const string kSettingsPrefix;

string demangle(const char* name);

template <class T> string getClassName(const T& t);
template <class T> string getClassName();

float alignf(float f);

ofVec3f randVec3f();

ofVec3f project(const ofVec3f& obj);
ofVec3f unproject(const ofVec2f& screen);

namespace color {
    extern const ofColor kMain;
    extern const ofColor kPalePinkLight;
    extern const ofColor kPalePinkHeavy;
    extern const ofColor kDarkPinkLight;
    extern const ofColor kDarkPinkHeavy;
}

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

DP_SCORE_NAMESPACE_END

#define dpDebugFunc() ofLogVerbose() << getClassName(*this) << ", " << __FUNCTION__

OFX_MOTIONER_NAMESPACE_BEGIN

string getJointNameLower(int index);

OFX_MOTIONER_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreCommon__) */
