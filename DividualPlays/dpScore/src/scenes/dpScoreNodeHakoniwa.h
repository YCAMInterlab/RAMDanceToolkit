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

    void setX(int i) { mX = ofClamp(i, 0, kNumX); };
    void setY(int i) { mY = ofClamp(i, 0, kNumY); };
    void setFocus(bool focus) { mFocus = true; }
    
private:
    Desk mDesk;
    bool mFocus {true};
    int mX {1};
    int mY {0};
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeHakoniwa_h */
