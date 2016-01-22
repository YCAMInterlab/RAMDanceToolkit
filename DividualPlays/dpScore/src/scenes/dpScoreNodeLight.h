//
//  dpScoreNodeLight.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeLight_h
#define dpScoreNodeLight_h

#include "dpScoreCommon.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeLight: public ofNode {
public:
	NodeLight();
	virtual ~NodeLight();

	void customDraw() override;

    constexpr static const float getHeight() { return 800.f; }
    
private:
    void drawParLight(float angle);
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeLight_h */
