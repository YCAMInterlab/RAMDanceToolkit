//
//  dpScoreNodeCameraUnit.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeCameraUnit_h
#define dpScoreNodeCameraUnit_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeCameraUnit: public BaseNode {
public:
    NodeCameraUnit();
    virtual ~NodeCameraUnit();
    
    void customDraw() override;
    static float getWidth()
    {
        return 58.f;
    }
    static float getHeight()
    {
        return 106.f;
    }
    static float getDepth()
    {
        return 61.f;
    }
    
    constexpr static const float kMaxMacBookAngle {110.f};
    
private:
    MacBook mMBP;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeCameraUnit_h */
