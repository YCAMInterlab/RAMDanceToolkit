//
//  dpScoreFlowChartHakoniwa.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/4/16.
//
//

#include "dpScoreFlowChartHakoniwa.h"
#include "dpScoreStyle.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeHakoniwa.h"
#include "dpScoreNodeStage.h"

DP_SCORE_NAMESPACE_BEGIN

HakoTornado::HakoTornado()
{
	mBack.setup(ofVec3f::zero(), getWidth(), getHeight(), 1.f);
	mSide.setup(ofVec3f(getWidth(), 0.f, 0.f), 1.f, getHeight(), getDepth());
	const float r {5.f};
	const float h {25.f};
	mCylinder.setup(ofVec3f(getWidth() * 0.5f, h * 0.5f, getDepth() * 0.5f), r, h);
}

void HakoTornado::customDraw()
{
	mCylinder.draw();
	mBack.draw();
	mSide.draw();
}

float HakoTornado::getWidth()
{
	return 26.f;
}

float HakoTornado::getHeight()
{
	return 35.f;
}

float HakoTornado::getDepth()
{
	return 26.f;
}

#pragma mark ___________________________________________________________________

HakoWorm::HakoWorm()
{
	mBox.setup(ofVec3f::zero(), getWidth(), getHeight(), getDepth());
}

void HakoWorm::customDraw()
{
	mBox.draw();
}

float HakoWorm::getWidth()
{
	return 30.f;
}

float HakoWorm::getHeight()
{
	return 6.f;
}

float HakoWorm::getDepth()
{
	return 15.f;
}

#pragma mark ___________________________________________________________________

HakoStruggle::HakoStruggle()
{
	mBack.setup(ofVec3f::zero(), getWidth(), getHeight(), 1.f);
}

void HakoStruggle::customDraw()
{
	mBack.draw();
}

float HakoStruggle::getWidth()
{
	return 34.f;
}

float HakoStruggle::getHeight()
{
	return 27.f;
}

float HakoStruggle::getDepth()
{
	return 1.f;
}

#pragma mark ___________________________________________________________________

HakoServoPendulum::HakoServoPendulum()
{
	mBack.setup(ofVec3f::zero(), getWidth(), getHeight(), 1.f);
}

void HakoServoPendulum::customDraw()
{
	mBack.draw();
}

float HakoServoPendulum::getWidth()
{
	return 47.f;
}

float HakoServoPendulum::getHeight()
{
	return 31.f;
}

float HakoServoPendulum::getDepth()
{
	return 1.f;
}

#pragma mark ___________________________________________________________________

//22 20 14 13
HakoGear::HakoGear()
{
	const float d {2.f};
	mBotom.setup(ofVec3f::zero(), getWidth(), d, getDepth());
	mFront.setup(ofVec3f::zero(), getWidth(), getHeight(), d);
	mGears.clear();
	mGears.push_back(Gear::create(ofVec3f(3.f, getHeight() - 2.f, -d * 2.f),
                                  22.f, d, 20.f, 6, 12.f));
	mGears.push_back(Gear::create(ofVec3f(getWidth() - 11.f, getHeight() - 12.f, -d),
                                  20.f, d, -30.f, 6, 12.f));
	mGears.push_back(Gear::create(ofVec3f(19.f, 27.f, -d),
                                  14.f, d, 40.f, 8, 9.f));
	mGears.push_back(Gear::create(ofVec3f(33.f, 37.f, -d * 2.f),
                                  13.f, d, -50.f, 8, 9.f));
	mGears.push_back(Gear::create(ofVec3f(39.f, 17.f, -d),
                                  13.f, d, 60.f, 8, 9.f));
}

void HakoGear::customDraw()
{
	mBotom.draw();
	mFront.draw();
	for (auto& g : mGears) {
		g.draw();
	}
}

float HakoGear::getWidth()
{
	return 44.f;
}

float HakoGear::getHeight()
{
	return 65.f;
}

float HakoGear::getDepth()
{
	return 26.f;
}

#pragma mark ___________________________________________________________________

HakoMagPendulum::HakoMagPendulum()
{
	mBox.setup(ofVec3f::zero(), getWidth(), getHeight(), getDepth());
	mMag.setup(ofVec3f::zero(), 2.f, 4.f, 8);
	mLine.setup(ofVec3f::zero(), ofVec3f::zero(), 5);
}

void HakoMagPendulum::customDraw()
{
	mBox.draw();
	ofVec3f v(getWidth() * 0.5f, getHeight() + 5.f, getDepth() * 0.5f);
	const float t {getElapsedTime()};
	const float r {getWidth() * 0.3f};
	v.x += ::cos(t * 2.1f) * r;
	v.z += ::sin(t * 3.3f) * r;
	{
		ScopedMatrix m;
		ofTranslate(v);
		mMag.draw();
	}
	mLine.update(v, ofVec3f(0.f, NodeStage::kHeight, 0.f));
	{
		ScopedStyle s;
		ofSetColor(128);
		mLine.draw();
	}
}

float HakoMagPendulum::getWidth()
{
	return 45.f;
}

float HakoMagPendulum::getHeight()
{
	return 20.f;
}

float HakoMagPendulum::getDepth()
{
	return 45.f;
}

#pragma mark ___________________________________________________________________

HakoSandStorm::HakoSandStorm()
{
	mBox.setup(ofVec3f::zero(), getWidth(), getHeight(), getDepth());
	mPlate.setup(ofVec3f::zero(), getWidth() - 3.f, 5.5f, getDepth() - 3.f);
}

void HakoSandStorm::customDraw()
{
	mBox.draw();
	{
		ScopedMatrix m;
		ofTranslate(1.5f, getHeight() - 5.5f, 1.5f);
		ofTranslate(mPlate.getWidth() * 0.5f, 0.f, mPlate.getDepth() * 0.5f);
		const float rx {::cosf(getElapsedTime() * 1.f) * 5.f};
		const float rz {::sinf(getElapsedTime() * 1.3f) * 5.f};
		ofRotateZ(rz);
		ofRotateX(rx);
		ofTranslate(-mPlate.getWidth() * 0.5f, 0.f, -mPlate.getDepth() * 0.5f);
		mPlate.draw();
	}
}

float HakoSandStorm::getWidth()
{
	return 58.f;
}

float HakoSandStorm::getHeight()
{
	return 25.f;
}

float HakoSandStorm::getDepth()
{
	return 58.f;
}

DP_SCORE_NAMESPACE_END