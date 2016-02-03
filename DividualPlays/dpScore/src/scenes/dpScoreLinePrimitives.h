//
//  dpScoreLinePrimitives.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreLinePrimitives_h
#define dpScoreLinePrimitives_h

#include "dpScoreCompoundLine.h"

DP_SCORE_NAMESPACE_BEGIN

class Box: public CompoundLine {
public:
	static Box create(const ofVec3f& p, float w, float h, float d);
	void setup(const ofVec3f& p, float w, float h, float d);
};

class Cylinder: public CompoundLine {
public:
	static Cylinder create(const ofVec3f& p, float r, float h, int res = 16);
	void setup(const ofVec3f& p, float r, float h, int res = 16);
};

class Rect: public CompoundLine {
public:
	static Rect create(const ofVec3f& p, float w, float h);
	void setup(const ofVec3f& p, float w, float h);
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreLinePrimitives_h */
