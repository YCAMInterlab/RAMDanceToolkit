//
//  dpScoreNodeMotioner.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeMotioner_h
#define dpScoreNodeMotioner_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeMotioner: public BaseNode {
public:
    NodeMotioner();
    virtual ~NodeMotioner();
    
    void customDraw() override;
    
private:
    
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeMotioner_h */
