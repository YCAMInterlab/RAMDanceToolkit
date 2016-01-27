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

	static ofVec3f getFrontLeft();
	static ofVec3f getFrontRight();
	static float getFrontDisplayWidth();
	static float getFrontDisplayHeight();
	static float getFrontDisplayDepth();
	static float getFrontDisplayAngle();

	static ofVec3f getRearLeft();
	static ofVec3f getRearRight();
	static float getRearDisplayWidth();
	static float getRearDisplayHeight();
	static float getRearDisplayDepth();
	static float getRearDisplayAngle();

	ofFbo fbo;
    
private:
    vector<Box> mFronts;
    vector<Box> mRears;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeDisplay_h */
