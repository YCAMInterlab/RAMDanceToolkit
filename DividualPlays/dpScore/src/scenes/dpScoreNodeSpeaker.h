//
//  dpScoreNodeSpeaker.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeSpeaker_h
#define dpScoreNodeSpeaker_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeSpeaker: public BaseNode {
public:
    NodeSpeaker();
    virtual ~NodeSpeaker();
    
    void customDraw() override;
    
    static float getX();
    static float getH();
    
    const int kNumSpeakers {6};
    
private:
    vector<Speaker> mSpeakers;
    Cylinder mPole;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeSpeaker_h */
