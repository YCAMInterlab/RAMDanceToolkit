//
//  dpScoreCompound.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompound.h"

DP_SCORE_NAMESPACE_BEGIN

static bool _enableCollapse = false;

void compoundSetEnableCollapse(bool enable)
{
	_enableCollapse = enable;
}

bool compoundGetEnableCollapse()
{
	return _enableCollapse;
}

static const float _s {30.f};

PrimitiveTypeBase::PrimitiveTypeBase() :
	mRot(ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s),
	mSpd(ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s),
	mAng(0.f),
	mPos(0.f)
{
}

void PrimitiveTypeBase::update(float time)
{
	mAng = mRot * time;
	mPos = mSpd * time;
	mPos.y += 10.f * time;
}

void PrimitiveTypeBase::transformGL()
{
	ofTranslate(mPos);
	ofRotateZ(mAng.z);
	ofRotateY(mAng.y);
	ofRotateX(mAng.x);
}

void PrimitiveTypeBase::reset()
{
	mRot.set(ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s);
	mSpd.set(ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s, ofRandom(-1.f, 1.f) * _s);
	mAng.set(0.f);
	mPos.set(0.f);
}

DP_SCORE_NAMESPACE_END