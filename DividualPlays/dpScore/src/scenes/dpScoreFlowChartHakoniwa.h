//
//  dpScoreFlowChartHakoniwa.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/4/16.
//
//

#ifndef dpScoreFlowChartHakoniwa_h
#define dpScoreFlowChartHakoniwa_h

#include "dpScoreCommon.h"
#include "dpScoreLinePrimitives.h"
#include "dpScoreComplexLineObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class BaseHakoniwa: public ofNode {
public:
	virtual ~BaseHakoniwa() = default;
    virtual float getWidth() { return 0.f; }
    virtual float getHeight() { return 0.f; }
    virtual float getDepth() { return 0.f; }
};

#pragma mark ___________________________________________________________________

class HakoTornado final: public BaseHakoniwa {
public:
	HakoTornado();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBack, mSide;
	Cylinder mCylinder;
};

#pragma mark ___________________________________________________________________

class HakoWorm final: public BaseHakoniwa {
public:
	HakoWorm();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBox;
};

#pragma mark ___________________________________________________________________

class HakoStruggle final: public BaseHakoniwa {
public:
	HakoStruggle();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBack;
};

#pragma mark ___________________________________________________________________

class HakoServoPendulum final: public BaseHakoniwa {
public:
	HakoServoPendulum();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBack;
};

#pragma mark ___________________________________________________________________

class HakoGear final: public BaseHakoniwa {
public:
	HakoGear();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
    Box mFront;
    Box mBotom;
    vector<Gear> mGears;
};

#pragma mark ___________________________________________________________________

class HakoMagPendulum final: public BaseHakoniwa {
public:
	HakoMagPendulum();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBox;
    Cylinder mMag;
    Line mLine;
};

#pragma mark ___________________________________________________________________

class HakoSandStorm final: public BaseHakoniwa {
public:
	HakoSandStorm();
	void customDraw() override;
	float getWidth() override;
	float getHeight() override;
	float getDepth() override;
private:
	Box mBox;
    Box mPlate;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreFlowChartHakoniwa_h */
