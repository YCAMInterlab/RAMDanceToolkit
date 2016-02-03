//
//  dpScoreCompoundLine.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompoundLine.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

Line Line::make(const ofVec3f& p0, const ofVec3f& p1)
{
	Line line;
	line.c = p0 * 0.5f + p1 * 0.5f;
	line.v = p1 - p0;
	return line;
}

void Line::set(const ofVec3f& p0, const ofVec3f& p1)
{
	c = p0 * 0.5f + p1 * 0.5f;
	v = p1 - p0;
}

void Line::draw()
{
	ofLine(c - v * 0.5f, c + v * 0.5f);
}

bool CompoundLine::enableAnimation = false;

void CompoundLine::update()
{
	if (enableAnimation) {
		mUpdate = true;
		const double d {ofGetLastFrameTime()};
		for (auto& p : mPoints) {
			p.ang += p.rot * d;
			p.pos += p.spd * d;
			p.pos.y += 10.f * d;
		}
	}
	else {
		if (mUpdate) {
			reset();
		}
		mUpdate = false;
	}
}

void CompoundLine::draw()
{
	update();

	for (auto i : rep(mLines.size())) {
		ScopedMatrix m;
		if (enableAnimation) {
			auto& p = mPoints.at(i);
			ofTranslate(p.pos);
			ofRotateZ(p.ang.z);
			ofRotateY(p.ang.y);
			ofRotateX(p.ang.x);
		}
		mLines.at(i).draw();
	}
}

void CompoundLine::reset()
{
	const float s {30.f};
	for (auto& p : mPoints) {
		p.rot = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
		p.spd = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
		p.ang.set(0.f);
		p.pos.set(0.f);
	}
}

DP_SCORE_NAMESPACE_END