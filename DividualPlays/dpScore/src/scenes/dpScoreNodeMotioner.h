//
//  dpScoreNodeMotioner.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeMotioner_h
#define dpScoreNodeMotioner_h

#include "dpScoreCommon.h"
#include "dpScoreObjects.h"
#include "ofxMotioner.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeMotioner: public ofNode {
public:
    NodeMotioner();
    virtual ~NodeMotioner();
    
    void customDraw() override;
    
private:
    
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeMotioner_h */
