//
//  dpScoreNodeMasterHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeMasterHakoniwa_h
#define dpScoreNodeMasterHakoniwa_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeMasterHakoniwa: public BaseNode {
public:
    NodeMasterHakoniwa();
    virtual ~NodeMasterHakoniwa();
    
    void customDraw() override;
    
private:
    ofVbo mWaterTank;
    Box mFront;
    Box mRear;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeMasterHakoniwa_h */
