//
//  dpScoreBaseNode.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/23/16.
//
//

#include "dpScoreBaseNode.h"

DP_SCORE_NAMESPACE_BEGIN

BaseNode::BaseNode()
{
    mOffsets.clear();
}

void BaseNode::customDraw()
{
}

size_t BaseNode::getNumAimingPositions() const
{
    return mOffsets.size();
}

ofVec3f BaseNode::getAimingPosition(size_t idx) const
{
    return getGlobalPosition() + mOffsets.at(idx);
}

void BaseNode::addAimingOffset(const ofVec3f& offset)
{
    mOffsets.push_back(offset);
}

void BaseNode::clearAimingOffsets()
{
    mOffsets.clear();
}

ofCamera& BaseNode::getCamera()
{
    return mCamera;
}

const ofCamera& BaseNode::getCamera() const
{
    return mCamera;
}

DP_SCORE_NAMESPACE_END