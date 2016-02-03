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

Line Line::make(const ofVec3f& p0, const ofVec3f& p1)
{
	Line line;
	line.c = p0 * 0.5f + p1 * 0.5f;
	line.v = p1 - p0;
	return line;
}

void Line::set(const ofVec3f& p0, const ofVec3f& p1)
{
	c = p0 * 0.5f + p1 * 0.5f;
	v = p1 - p0;
}

void Line::draw()
{
	ofLine(c - v * 0.5f, c + v * 0.5f);
}

DP_SCORE_NAMESPACE_END