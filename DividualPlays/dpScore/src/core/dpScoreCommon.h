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
#include "dpScorePP.h"
#include "dpScoreRTTI.h"
#include "dpScoreTypes.h"
#include "dpScoreAlign.h"
#include "dpScoreUtils.h"

// WIP
//#define USE_CUSTOM_MEMORY_ALLOCATOR

//#ifdef USE_CUSTOM_MEMORY_ALLOCATOR
//void* operator new(size_t size) throw(std::bad_alloc);
//void* operator new[](size_t size) throw(std::bad_alloc);
//void operator delete(void* pv) throw();
//void operator delete[](void* pv) throw();
//#endif


DP_SCORE_NAMESPACE_BEGIN

extern const int kW;
extern const int kH;

extern const int kFrameRate;

extern const int kMaxSkeleton;

extern const int kOscClientPort;

extern const string kOscAddrChangeScene;
extern const string kOscAddrSensorScale;
extern const string kOscAddrNumHakoniwaRemained;

extern const string kOscAddrCameraUnitVector;
extern const string kOscAddrCameraUnitVectorTotal;
extern const string kOscAddrCameraUnitVectorFeatures;
extern const string kOscAddrCaneraUnitVecContourBlob;
extern const string kOscAddrCameraUnitMean;
extern const string kOscAddrCameraUnitPixelateR;
extern const string kOscAddrCameraUnitPixelateG;
extern const string kOscAddrCameraUnitPixelateB;

extern const string kOscAddrMotioner;

extern const string kSettingsDir;
extern const string kSettingsPrefix;

extern const string kXmlSettingsPath;
extern const string kXmlSettingsPathFlowChart;

extern const int kNumCameraunitVectors;

extern const string kFontPath;
extern const string kFontPathJP;

extern const float kFlowChartLineWidth;


DP_SCORE_NAMESPACE_END


OFX_MOTIONER_NAMESPACE_BEGIN

string getJointNameLower(int index);

OFX_MOTIONER_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreCommon__) */
