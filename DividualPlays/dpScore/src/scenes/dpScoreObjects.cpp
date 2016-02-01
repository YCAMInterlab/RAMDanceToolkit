//
//  dpScoreObjects.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreObjects.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

bool LineObj::enableAnimation = false;

void LineObj::update()
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

void LineObj::draw()
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

void LineObj::reset()
{
	const float s {30.f};
	for (auto& p : mPoints) {
		p.rot = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
		p.spd = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
		p.ang.set(0.f);
		p.pos.set(0.f);
	}
}

void Box::setup(const ofVec3f& p, float w, float h, float d)
{
	const ofVec3f o(p.x + w * 0.5f, p.y + h * 0.5f, p.z + d * 0.5);
	const float x {w * 0.5f};
	const float y {h * 0.5f};
	const float z {d * 0.5f};
	const ofVec3f v0 {ofVec3f(-x, -y, -z) + o};
	const ofVec3f v1 {ofVec3f(x, -y, -z) + o};
	const ofVec3f v2 {ofVec3f(x, -y,  z) + o};
	const ofVec3f v3 {ofVec3f(-x, -y,  z) + o};
	const ofVec3f v4 {ofVec3f(-x,  y, -z) + o};
	const ofVec3f v5 {ofVec3f(x,  y, -z) + o};
	const ofVec3f v6 {ofVec3f(x,  y,  z) + o};
	const ofVec3f v7 {ofVec3f(-x,  y,  z) + o};

	mLines.clear();
	mLines.push_back(Line::make(v0, v1));
	mLines.push_back(Line::make(v1, v2));
	mLines.push_back(Line::make(v2, v3));
	mLines.push_back(Line::make(v3, v0));

	mLines.push_back(Line::make(v0, v4));
	mLines.push_back(Line::make(v1, v5));
	mLines.push_back(Line::make(v2, v6));
	mLines.push_back(Line::make(v3, v7));

	mLines.push_back(Line::make(v4, v5));
	mLines.push_back(Line::make(v5, v6));
	mLines.push_back(Line::make(v6, v7));
	mLines.push_back(Line::make(v7, v4));

	mPoints.assign(mLines.size(), Point());
	reset();
}

void Cylinder::setup(const ofVec3f& p, float r, float h)
{
	mCenter = p;
	mRadius = r;
	mHeight = h;

	const ofVec3f o(p.x + r, p.y + h * 0.5f, p.z + r);
	const float x {r};
	const float y {h * 0.5f};
	const float z {r};
	const ofVec3f v0 {ofVec3f(-x, -y, -z) + o};
	const ofVec3f v1 {ofVec3f(x, -y, -z) + o};
	const ofVec3f v2 {ofVec3f(x, -y,  z) + o};
	const ofVec3f v3 {ofVec3f(-x, -y,  z) + o};
	const ofVec3f v4 {ofVec3f(-x,  y, -z) + o};
	const ofVec3f v5 {ofVec3f(x,  y, -z) + o};
	const ofVec3f v6 {ofVec3f(x,  y,  z) + o};
	const ofVec3f v7 {ofVec3f(-x,  y,  z) + o};

	mLines.clear();
	mLines.push_back(Line::make(v0, v1));
	mLines.push_back(Line::make(v1, v2));
	mLines.push_back(Line::make(v2, v3));
	mLines.push_back(Line::make(v3, v0));

	mLines.push_back(Line::make(v0, v4));
	mLines.push_back(Line::make(v1, v5));
	mLines.push_back(Line::make(v2, v6));
	mLines.push_back(Line::make(v3, v7));

	mLines.push_back(Line::make(v4, v5));
	mLines.push_back(Line::make(v5, v6));
	mLines.push_back(Line::make(v6, v7));
	mLines.push_back(Line::make(v7, v4));

	mPoints.assign(mLines.size(), Point());
	reset();
}

void Cylinder::draw()
{
	if (LineObj::enableAnimation) {
		LineObj::draw();
	}
	else {
		ofDrawCylinder(mCenter, mRadius, mHeight);
	}
}

void Rect::setup(const ofVec3f& p, float w, float h)
{
	const float x {w * 0.5f};
	const float y {h * 0.5f};
	const ofVec3f v0 {ofVec3f(-x, -y, 0.f) + p};
	const ofVec3f v1 {ofVec3f(x, -y, 0.f) + p};
	const ofVec3f v2 {ofVec3f(x,  y, 0.f) + p};
	const ofVec3f v3 {ofVec3f(-x,  y, 0.f) + p};

	mLines.clear();
	mLines.push_back(Line::make(v0, v1));
	mLines.push_back(Line::make(v1, v2));
	mLines.push_back(Line::make(v2, v3));
	mLines.push_back(Line::make(v3, v0));

	mPoints.assign(mLines.size(), Point());
	reset();
}

Desk::Desk()
{
	//const float tableDim {getDimension()};
	const float w {getWidth()};
	const float d {getDepth()};
	const float h {getHeight()};
	const float thickness {11.f};
	const float leg {5.f};
	mBoxes.clear();
	mBoxes.push_back(Box::create(ofVec3f(0.f, h - thickness, 0.f), w, thickness, d));
	mBoxes.push_back(Box::create(ofVec3f(0.f, 0.f, 0.f), leg, h - thickness, leg));
	mBoxes.push_back(Box::create(ofVec3f(w - leg, 0.f, 0.f), leg, h - thickness, leg));
	mBoxes.push_back(Box::create(ofVec3f(w - leg, 0.f, d - leg), leg, h - thickness, leg));
	mBoxes.push_back(Box::create(ofVec3f(0.f, 0.f, d - leg), leg, h - thickness, leg));
}

void Desk::draw()
{
	for (auto& b : mBoxes) {
		b.draw();
	}
}

Deck::Deck()
{
	const float w {getWidth()};
	const float d {getDepth()};
	const float h {getHeight()};
	const float thickness {11.f};
	mTop.setup(ofVec3f(0.f, h - thickness, 0.f), w, thickness, d);

	const float legRad {2.5f};
	const float legH {h - thickness};
	mLegs.clear();
	mLegs.push_back(Cylinder::create(ofVec3f(legRad, legH * 0.5f, legRad), legRad, legH));
	mLegs.push_back(Cylinder::create(ofVec3f(w - legRad, legH * 0.5f, legRad), legRad, legH));
	mLegs.push_back(Cylinder::create(ofVec3f(legRad, legH * 0.5f, d - legRad), legRad,  legH));
	mLegs.push_back(Cylinder::create(ofVec3f(w - legRad, legH * 0.5f, d - legRad), legRad, legH));
}

void Deck::draw()
{
	ScopedStyle s;
	ofSetCylinderResolution(8, 1);

	mTop.draw();
	for (auto& l : mLegs) {
		l.draw();
	}
}

void Chair::draw()
{
	ScopedStyle s;
	drawBox(ofVec3f::zero(), 4.f, 1.5f, 35.f);
	drawBox(ofVec3f(42.f - 4.f, 0.f, 0.f), 4.f, 1.5f, 35.f);

	drawBox(ofVec3f(0.f, 0.f, 35.f - 1.5f), 4.f, 34.f, 1.5f);
	drawBox(ofVec3f(42.f - 4.f, 0.f, 35.f - 1.5f), 4.f, 34.f, 1.5f);
	{
		ScopedMatrix m;
		ofTranslate(0.f, 34.f, 0.f);
		drawBox(ofVec3f::zero(), 42.f, 5.f, 35.f);
		ofTranslate(0.f, 0.f, 35.f);

		{
			ScopedTranslate t(0.f, 22.f, 0.f);
			drawBox(ofVec3f::zero(), -4.f, 1.5f, -32.f);
			drawBox(ofVec3f(42.f, 0.f, 0.f), 4.f, 1.5f, -32.f);
		}

		ofTranslate(0.f, 5.f, 0.f);
		ofRotateX(10.f);
		drawBox(ofVec3f::zero(), 42.f, 53.f, 5.f);
	}
}

MacBook::MacBook()
{
	mBottom.setup(ofVec3f::zero(), getWidth(), 1.f, getDepth());
	mTop.setup(ofVec3f::zero(), getWidth(), 0.5f, -getDepth());
}

void MacBook::draw()
{
	ScopedStyle s;
	mBottom.draw();
	ScopedMatrix m;
	ofTranslate(0.f, 0.f, getDepth());
	ofRotateX(angle);
	mTop.draw();

	if (!LineObj::enableAnimation && drawDisplay && angle >= 15.f) {
		ScopedStyle s;
		ofRotateX(90.f);
		ofFill();
		{
			ScopedTranslate t(0.f, -getDepth(), 0.5f);
			ofSetColor(color::kMain);
			drawDisplay(getWidth(), getDepth());
		}
		{
			ScopedTranslate t(0.f, -getDepth(), -0.5f);
			ofSetColor(ofColor::black);
			ofRect(0.f, 0.f, getWidth(), getDepth());
		}
	}
}

DP_SCORE_NAMESPACE_END