//
//  dpScoreFlowChartBaseScene.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/16.
//
//

#ifndef dpScoreFlowChartBaseScene_h
#define dpScoreFlowChartBaseScene_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class FlowChartBaseScene {
public:
	typedef FlowChartBaseScene super;
    virtual ~FlowChartBaseScene() = default;
	void setup(float move, float idle, float line, float total, bool easeyCam = false);
	virtual void reset(class SceneFlowChart* owner);
	virtual void update(class SceneFlowChart* owner);
	virtual void drawText(class SceneFlowChart* owner)
	{
	}

	ofNode& getNodeCenter()
	{
        return mNodeCenter;
	}
	ofPtr<ofCamera> getCamera()
	{
		return mCamera;
	}
	float getTotalTime() const
	{
		return mTotalTime;
	}
	float getMoveSpan() const
	{
		return mMoveSpan;
	}
	float getIdleSpan() const
	{
		return mIdleSpan;
	}
	float getLineSpan() const
	{
		return mLineSpan;
	}
	float getElapsedTime() const
	{
		return mElapsedTime;
	}
	float getLineTime() const
	{
		return mLineTime;
	}

protected:
	ofNode mNodeCenter;
	ofPtr<ofCamera> mCamera {ofPtr<ofCamera>(new ofCamera())};
	float mMoveSpan {2.f};
	float mIdleSpan {2.f};
	float mLineSpan {1.f};
	float mTotalTime {60.f};
	float mElapsedTime {0.f};
	float mLineTime {0.f};
};

DP_SCORE_NAMESPACE_END


#endif /* dpScoreFlowChartBaseScene_h */
