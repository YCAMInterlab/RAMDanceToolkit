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
	NodeDancer()
	{
		addAimingOffset(ofVec3f::zero());
        
        getCamera().setFov(50.f);
        getCamera().setPosition(0.f, 200.f, 800.f);
        getCamera().setOrientation(ofVec3f(-20.f, 0.f, 0.f));
	}

	virtual ~NodeDancer()
	{
	}

	void customDraw() override
	{
	}

private:

};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeDancer_h */
