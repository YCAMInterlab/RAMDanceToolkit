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

class LineType : public PrimitiveTypeBase {
public:
	LineType(const ofVec3f &v0, const ofVec3f &v1);
	virtual ~LineType();
	LineType(const LineType &) = default;
	LineType& operator = (const LineType&) = default;
	void set(const ofVec3f& v0, const ofVec3f& v1);
	void draw();
    
protected:
	ofVec3f mV0, mV1;
};

typedef Compound<LineType> CompoundLine;


DP_SCORE_NAMESPACE_END

#endif /* dpScoreCompoundLine_h */
