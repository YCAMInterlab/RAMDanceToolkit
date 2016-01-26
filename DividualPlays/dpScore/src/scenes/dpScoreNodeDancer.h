//
//  dpScoreNodeDancer.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/23/16.
//
//

#ifndef dpScoreNodeDancer_h
#define dpScoreNodeDancer_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeDancer: public BaseNode {
public:
    NodeDancer();
    virtual ~NodeDancer();

    void customDraw() override;

};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeDancer_h */
