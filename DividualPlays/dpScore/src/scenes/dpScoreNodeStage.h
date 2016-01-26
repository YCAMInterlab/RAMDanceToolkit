//
//  dpScoreNodeStage.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

#ifndef dpScoreNodeStage_h
#define dpScoreNodeStage_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeStage: public BaseNode {
public:
	NodeStage();
	virtual ~NodeStage();

	void customDraw() override;

	constexpr static float kWidth {800.f};
	constexpr static float kDepth {800.f};
	constexpr static float kHeight {740.f};
	constexpr static float kScrH {kWidth * (9.f / 16.f)};

    ofFbo* fbo {nullptr};
    
private:
	ofVbo mFloor;
    Rect mScreenRect;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeStage_h */
