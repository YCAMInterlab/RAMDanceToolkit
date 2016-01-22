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

extern const int kMaxSkeleton;

extern const int kOscClientPort;

extern const string kOscAddrChangeScene;
extern const string kOscAddrSensorScale;

extern const string kOscAddrCameraUnitVector;
extern const string kOscAddrCameraUnitVectorTotal;
extern const string kOscAddrCameraUnitVectorFeatures;
extern const string kOscAddrCaneraUnitVecContourBlob;
extern const string kOscAddrCameraUnitMean;
extern const string kOscAddrCameraUnitPixelateR;
extern const string kOscAddrCameraUnitPixelateG;
extern const string kOscAddrCameraUnitPixelateB;

extern const string kOscAddrMotioner;

extern const string kEventAddrChangeScene;

extern const string kSettingsDir;
extern const string kSettingsPrefix;

extern const string kXmlSettingsPath;

extern const int kNumCameraunitVectors;

extern const string kFontPath;

extern const float kFlowChartLineWidth;

string demangle(const char* name);

template <class T> string getClassName(const T& t);
template <class T> string getClassName();

float getLineUped(float length,
                  int index,
                  int total,
                  bool fromCenter = true);

float clamp(float f, float range = 1.f);

float aligned(float f);

void alignedLine(const ofPoint& p1, const ofPoint& p2);
void alignedLine(float x1, float y1, float x2, float y2);
void alignedLine(float x1, float y1, float z1, float x2, float y2, float z2);

void alignedRect(const ofRectangle& r);
void alignedRect(const ofPoint& p, float w, float h);
void alignedRect(float x, float y, float w, float h);
void alignedRect(float x, float y, float z, float w, float h);

void alignedTranslate(const ofPoint& p);
void alignedTranslate(float x, float y, float z = 0.f);

ofVec2f alignedVec2f(const ofVec2f& p);
ofVec2f alignedVec2f(float x, float y);

ofVec3f alignedVec3f(const ofVec3f& p);
ofVec3f alignedVec3f(float x, float y, float z = 0.f);

ofRectangle alignedRectangle(float px, float py, float w, float h);
ofRectangle alignedRectangle(const ofPoint& p, float w, float h);

ofVec3f randVec3f();

ofVec3f project(const ofVec3f& obj);
ofVec3f unproject(const ofVec2f& screen);

void billboard();

template<class SceneClass>
void notifyChangeScene()
{
    ofxEventMessage m;
    m.setAddress(kEventAddrChangeScene);
    const string className = getClassName<SceneClass>();
    m.addStringArg(className);
    ofxNotifyEvent(m);
}

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

typedef unsigned char byte;

union floatByte {
    float f;
    byte b[4];
};

union floatInt {
    float f;
    int i;
};

union intByte {
    int i;
    byte b[4];
};

struct OnOffNumpunct : numpunct<char> {
    using numpunct<char>::string_type;
    string_type do_truename()  const { return "on "; }
    string_type do_falsename() const { return "off"; }
};

DP_SCORE_NAMESPACE_END

#define dpDebugFunc() ofLogVerbose() << getClassName(*this) << ", " << __FUNCTION__

OFX_MOTIONER_NAMESPACE_BEGIN

string getJointNameLower(int index);

OFX_MOTIONER_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreCommon__) */
