//
//  dpScoreCompoundLine.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompoundLine.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

LineType::LineType(const ofVec3f& v0, const ofVec3f& v1) :
	mV0(v0),
	mV1(v1)
{

}

LineType::~LineType()
{

}

void LineType::set(const ofVec3f& v0, const ofVec3f& v1)
{
	mV0 = v0;
	mV1 = v1;
}

void LineType::draw()
{
	if (compoundGetEnableCollapse()) {
		ScopedMatrix m;
		transformGL();
		ofLine(mV0, mV1);
	}
	else {
		ofLine(mV0, mV1);
	}
}

DP_SCORE_NAMESPACE_END