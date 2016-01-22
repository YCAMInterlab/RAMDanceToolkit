//
//  dpScoreNodeMasterHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeMasterHakoniwa_h
#define dpScoreNodeMasterHakoniwa_h

#include "dpScoreCommon.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeMasterHakoniwa: public ofNode {
public:
    NodeMasterHakoniwa();
    virtual ~NodeMasterHakoniwa();
    
    void customDraw() override;
    
private:
    ofVbo mWaterTank;
    Desk mDesk;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeMasterHakoniwa_h */
