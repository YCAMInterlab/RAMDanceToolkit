//
//  dpScoreNodeHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeHakoniwa_h
#define dpScoreNodeHakoniwa_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"


DP_SCORE_NAMESPACE_BEGIN

class NodeHakoniwa: public BaseNode {
public:
    NodeHakoniwa();
    virtual ~NodeHakoniwa();
    
    void customDraw() override;
    
    constexpr static const int kNumX { 3 };
    constexpr static const int kNumY { 2 };
    static float getWidth() { return Desk::getDimension() * kNumX;  }
    static float getDepth() { return Desk::getDimension() * kNumY + 90.f * (kNumY - 1);  }
private:
    Desk mDesk;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeHakoniwa_h */
