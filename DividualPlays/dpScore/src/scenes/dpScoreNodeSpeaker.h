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
    
    static float getWidth()
    {
        return 54.f;
    }
    static float getHeight()
    {
        return 130.f;
    }
    static float getDepth()
    {
        return 69.f;
    }
    
    static ofVec3f getLeft();
    static ofVec3f getRight();
    
private:
    
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeSpeaker_h */
