//
//  dpScoreNodeAudience.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeAudience_h
#define dpScoreNodeAudience_h

#include "dpScoreCommon.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeAudience: public ofNode {
public:
    NodeAudience();
    virtual ~NodeAudience();
    
    void customDraw() override;
    
private:
    Chair mChair;
    
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeAudience_h */
