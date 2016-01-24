//
//  dpScoreNodeDisplay.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeDisplay_h
#define dpScoreNodeDisplay_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeDisplay: public BaseNode {
public:
	NodeDisplay();
	virtual ~NodeDisplay();

	void customDraw() override;

	static ofVec3f getFrontLeft()
	{
		return ofVec3f(-NodeStage::kWidth * 0.5f - 61.f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
	}
	static ofVec3f getFrontRight()
	{
		return ofVec3f(NodeStage::kWidth * 0.5f + 61.f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
	}
    static float getFrontDisplayWidth()
    {
        return 104.f;
    }
    static float getFrontDisplayHeight()
    {
        return 61.4f;
    }
    static float getFrontDisplayDepth()
    {
        return 3.f;
    }

private:

};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeDisplay_h */
