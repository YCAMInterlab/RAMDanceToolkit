//
//  dpScoreStyle.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreStyle.h"

#include "dpScoreBaseNode.h"

DP_SCORE_NAMESPACE_BEGIN

void setColor(const BaseNode& n)
{
    ofSetColor(128 + 127 * n.t);
}

void setStyle(const BaseNode& n)
{
    ofSetLineWidth(kFlowChartLineWidth);
    setColor(n);
    ofNoFill();
    ofEnableDepthTest();
}

void setStyle()
{
    ofSetLineWidth(kFlowChartLineWidth);
    ofSetColor(ofColor::white);
    ofNoFill();
    ofEnableDepthTest();
}

DP_SCORE_NAMESPACE_END