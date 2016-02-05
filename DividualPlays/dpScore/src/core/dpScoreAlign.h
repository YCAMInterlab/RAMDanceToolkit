//
//  dpScoreAlign.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#ifndef dpScoreAlign_h
#define dpScoreAlign_h

#include "dpScorePP.h"

DP_SCORE_NAMESPACE_BEGIN

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

void beginAlign();
void endAlign();

struct ScopedAlign {
    ScopedAlign() { beginAlign(); }
    ~ScopedAlign() {endAlign(); }
    ScopedAlign(const ScopedAlign&) = delete;
    ScopedAlign& operator = (const ScopedAlign&) = delete;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreAlign_h */
