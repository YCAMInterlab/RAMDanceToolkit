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

LineType LineType::make(const ofVec3f& p0, const ofVec3f& p1)
{
	LineType line;
	line.c = p0 * 0.5f + p1 * 0.5f;
	line.v = p1 - p0;
	return line;
}

void LineType::set(const ofVec3f& p0, const ofVec3f& p1)
{
	c = p0 * 0.5f + p1 * 0.5f;
	v = p1 - p0;
}

void LineType::draw()
{
	ofLine(c - v * 0.5f, c + v * 0.5f);
}

DP_SCORE_NAMESPACE_END