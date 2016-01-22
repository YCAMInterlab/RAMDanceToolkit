//
//  dpScoreNodeStage.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

#ifndef dpScoreNodeStage_h
#define dpScoreNodeStage_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeStage: public ofNode {
public:
    NodeStage();
    virtual ~NodeStage();
    
	void customDraw() override;

	constexpr static const float kWidth {800.f};
	constexpr static const float kDepth {800.f};
	constexpr static const float kHeight {740.f};
    constexpr static const float kScrH {kWidth * (9.f / 16.f)};
private:
    ofVbo mFloor;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeStage_h */
