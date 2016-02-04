//
//  dpScoreCompoundLine.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreCompoundLine_h
#define dpScoreCompoundLine_h

#include "dpScoreCommon.h"
#include "dpScoreCompound.h"

DP_SCORE_NAMESPACE_BEGIN

// all object will make from OpenGL coord(righthanded)
struct LineType {
	static LineType make(const ofVec3f& p0, const ofVec3f& p1);
	void set(const ofVec3f& p0, const ofVec3f& p1);
	void draw();

	ofVec3f c;
	ofVec3f v;
};

typedef Compound<LineType> CompoundLine;

DP_SCORE_NAMESPACE_END

#endif /* dpScoreCompoundLine_h */
