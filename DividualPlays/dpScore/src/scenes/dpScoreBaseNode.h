//
//  dpScoreBaseNode.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/23/16.
//
//

#ifndef dpScoreBaseNode_h
#define dpScoreBaseNode_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class BaseNode : public ofNode {
public:
    BaseNode()
    {
        mOffsets.clear();
    }
    
    virtual ~BaseNode() {}
    virtual void customDraw() override {}
    
    size_t getNumAimingPositions() const { return mOffsets.size(); }
    ofVec3f getAimingPosition(size_t idx) const { return getGlobalPosition() + mOffsets.at(idx); }
    void addAimingOffset(const ofVec3f& offset) { mOffsets.push_back(offset); }
    void clearAimingOffsets() { mOffsets.clear(); }
    
    float t {0.f};
    
private:
    vector<ofVec3f> mOffsets;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreBaseNode_h */
