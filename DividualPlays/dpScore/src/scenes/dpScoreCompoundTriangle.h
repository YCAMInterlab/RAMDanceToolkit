//
//  dpScoreCompoundTriangle.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreCompoundTriangle_h
#define dpScoreCompoundTriangle_h

#include "dpScoreCommon.h"
#include "dpScoreCompound.h"

DP_SCORE_NAMESPACE_BEGIN

struct Triangle {
	static Triangle make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2);
	static Triangle make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2,
	                     const ofVec2f& _t0, const ofVec2f& _t1, const ofVec2f& _t2);
	void set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2);
	void set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2,
	         const ofVec2f& _t0, const ofVec2f& _t1, const ofVec2f& _t2);
	void draw();

	ofVec3f p0, p1, p2;
	ofVec2f t0, t1, t2;
	ofVec3f c;
};

typedef Compound<Triangle> CompoundTriangle;

DP_SCORE_NAMESPACE_END


#endif /* dpScoreCompoundTriangle_h */
