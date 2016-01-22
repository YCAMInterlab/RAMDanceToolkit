//
//  dpScoreScoped.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

#ifndef dpScoreScoped_h
#define dpScoreScoped_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

struct ScopedStyle {
    ScopedStyle() { ofPushStyle(); }
    virtual ~ScopedStyle() { ofPopStyle(); }
};

struct ScopedLineWidth : public ScopedStyle {
    ScopedLineWidth(float w) { ofSetLineWidth(w); }
    virtual ~ScopedLineWidth() {}
};

struct ScopedFill : public ScopedStyle {
    ScopedFill(bool bFill) { bFill ? ofFill() : ofNoFill(); }
    virtual ~ScopedFill() {}
};

struct ScopedBlendMode : public ScopedStyle {
    ScopedBlendMode(ofBlendMode mode) { ofEnableBlendMode(mode); }
    virtual ~ScopedBlendMode();
};

struct ScopedRectMode : public ScopedStyle {
    ScopedRectMode(ofRectMode mode) { ofSetRectMode(mode); }
    virtual ~ScopedRectMode();
};

struct ScopedColor : public ScopedStyle {
    ScopedColor(const ofColor& c) { ofSetColor(c); }
    ScopedColor(const ofColor& c, int a) { ofSetColor(c, a); }
    ScopedColor(int r, int g, int b) { ofSetColor(r, g, b); }
    ScopedColor(int gray, int a) { ofSetColor(gray, gray, gray, a); }
    ScopedColor(int r, int g, int b, int a) { ofSetColor(r, g, b, a); }
    virtual ~ScopedColor();
};

struct ScopedMatrix {
    ScopedMatrix() { ofPushMatrix(); }
    virtual ~ScopedMatrix() { ofPopMatrix(); }
};

struct ScopedTranslate : public ScopedMatrix {
    ScopedTranslate(const ofVec2f& v) { ofTranslate(v); };
    ScopedTranslate(const ofVec3f& v) { ofTranslate(v); };
    ScopedTranslate(float x, float y, float z = 0.f) { ofTranslate(x, y, z); };
    virtual ~ScopedTranslate() {}
};

struct ScopedScale : public ScopedMatrix {
    ScopedScale(float sx, float sy, float sz) { ofScale(sx, sy, sz); };
    ScopedScale(float s) { ofScale(s, s, s); };
    virtual ~ScopedScale() {}
};

struct ScopedRotate : public ScopedMatrix {
    ScopedRotate(float angle, float x, float y, float z) { ofRotate(angle, x, y, z); };
    virtual ~ScopedRotate() {}
};

struct ScopedRotateX : public ScopedMatrix {
    ScopedRotateX(float angle) { ofRotateX(angle); };
    virtual ~ScopedRotateX() {}
};

struct ScopedRotateY : public ScopedMatrix {
    ScopedRotateY(float angle) { ofRotateY(angle); };
    virtual ~ScopedRotateY() {}
};

struct ScopedRotateZ : public ScopedMatrix {
    ScopedRotateZ(float angle) { ofRotateZ(angle); };
    virtual ~ScopedRotateZ() {}
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreScoped_h */
