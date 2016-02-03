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
    
    static float getWidth();
    static float getHeight();
    static float getHeightTank();
    static float getDepth();
    static float getDepthFront();
    static float getDepthRear();
    
    const int kNumX{3};
    const int kNumZ{2};
    
private:
    ofVbo mWaterTank;
    Box mFrontBottom;
    Box mFrontTop;
    Box mRear;
    vector<Funnel> mFunnel;
    vector<Box> mMotors;
    vector<Cylinder> mValves;
    Water mWater;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeMasterHakoniwa_h */
