//
//  dpScoreCompoundTriangle.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompoundTriangle.h"

DP_SCORE_NAMESPACE_BEGIN

Triangle Triangle::make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p3)
{
    
}

void Triangle::set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2)
{
    p0 = _p0;
    p1 = _p1;
    p2 = _p2;
    c = (p0 + p1 + p2) / 3.f;
}

void Triangle::draw()
{
    
}

DP_SCORE_NAMESPACE_END