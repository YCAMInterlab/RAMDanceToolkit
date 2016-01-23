//
//  dpScoreNodeComputer.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeComputer_h
#define dpScoreNodeComputer_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeComputer: public BaseNode {
public:
    NodeComputer();
    virtual ~NodeComputer();
    
    void customDraw() override;
    
    float macAngle {0.f};
    
private:
    Deck mDeck;
    MacBook mMBP;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeComputer_h */
