//
//  dpScoreNodeComputer.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeComputer_h
#define dpScoreNodeComputer_h

#include "dpScoreCommon.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeComputer: public ofNode {
public:
    NodeComputer();
    virtual ~NodeComputer();
    
    void customDraw() override;
    
private:
    Deck mDeck;
    MacBook mMBP;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeComputer_h */
