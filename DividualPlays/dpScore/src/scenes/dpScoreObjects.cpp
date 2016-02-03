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

void Speaker::setup(const ofVec3f& p)
{
	const float w0 {getWidth()};
	const float w1 {23.f};
	const float w2 {35.f};
	const float h {getHeight()};
	const float d {getDepth()};

	const ofVec3f o(p.x + w0 * 0.5f, p.y + h * 0.5f, p.z + d * 0.5);
	const float x0 {w0 * 0.5f};
	const float x1 {w1 * 0.5f};
	const float x2 {w2 * 0.5f};
	const float y {h * 0.5f};
	const float z0 {d * 0.5f - 5.f};
	const float z1 {d * 0.5f};
	const float z2 {d * 0.5f};
	const ofVec3f v0 {ofVec3f(-x1, -y, -z1) + o};
	const ofVec3f v1 {ofVec3f(x1, -y, -z1) + o};
	const ofVec3f v2 {ofVec3f(x0, -y, z0) + o};
	const ofVec3f v3 {ofVec3f(-x0, -y, z0) + o};

	const ofVec3f v4 {ofVec3f(-x1,  y, -z1) + o};
	const ofVec3f v5 {ofVec3f(x1,  y, -z1) + o};
	const ofVec3f v6 {ofVec3f(x0,  y, z0) + o};
	const ofVec3f v7 {ofVec3f(-x0,  y, z0) + o};

	const ofVec3f v8 {ofVec3f(-x2, y, z2) + o};
	const ofVec3f v9 {ofVec3f(-x2, -y, z2) + o};
	const ofVec3f v10 {ofVec3f(x2, -y, z2) + o};
	const ofVec3f v11 {ofVec3f(x2, y, z2) + o};


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

	mLines.push_back(Line::make(v8, v9));
	mLines.push_back(Line::make(v9, v10));
	mLines.push_back(Line::make(v10, v11));
	mLines.push_back(Line::make(v11, v8));

	mLines.push_back(Line::make(v7, v8));
	mLines.push_back(Line::make(v3, v9));
	mLines.push_back(Line::make(v2, v10));
	mLines.push_back(Line::make(v6, v11));

	mPoints.assign(mLines.size(), Point());
	reset();
}

float Speaker::getWidth()
{
	return 42.f;
}
float Speaker::getHeight()
{
	return 55.f;
}
float Speaker::getDepth()
{
	return 30.f;
}

void Cylinder::setup(const ofVec3f& p, float r, float h, int res)
{
	for (auto i : rep(res)) {
		const float step {(float)TWO_PI / (float)res};
		const float rad0 {(i + 0) * step};
		const float rad1 {(i + 1) * step};
		const float x0 {::cosf(rad0) * r};
		const float z0 {::sinf(rad0) * r};
		const float x1 {::cosf(rad1) * r};
		const float z1 {::sinf(rad1) * r};

		// top circle
		mLines.push_back(Line::make(ofVec3f(x0, h * 0.5f, z0) + p, ofVec3f(x1, h * 0.5f, z1) + p));
		// bottom circle
		mLines.push_back(Line::make(ofVec3f(x0, -h * 0.5f, z0) + p, ofVec3f(x1, -h * 0.5f, z1) + p));
		// side face
		mLines.push_back(Line::make(ofVec3f(x0, -h * 0.5f, z0) + p, ofVec3f(x0, h * 0.5f, z0) + p));
	}

	mPoints.assign(mLines.size(), Point());
	reset();
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

void Funnel::setup(const ofVec3f& p, float r0, float r1, float h0, float h1)
{
	mLines.clear();
	auto res = 16;
	for (auto i : rep(res)) {
		const float step {(float)TWO_PI / (float)res};
		const float rad0 {(i + 0) * step};
		const float rad1 {(i + 1) * step};
		const float x0 {::cosf(rad0) * r0};
		const float z0 {::sinf(rad0) * r0};
		const float x1 {::cosf(rad1) * r0};
		const float z1 {::sinf(rad1) * r0};
		const float x2 {::cosf(rad0) * r1};
		const float z2 {::sinf(rad0) * r1};
		const float x3 {::cosf(rad1) * r1};
		const float z3 {::sinf(rad1) * r1};

		// top circle
		mLines.push_back(Line::make(ofVec3f(x0, 0.f, z0) + p, ofVec3f(x1, 0.f, z1) + p));
		// side face
		mLines.push_back(Line::make(ofVec3f(x0, 0.f, z0) + p, ofVec3f(x2, -h0, z2) + p));
		// bottom circle
		//mLines.push_back(Line::make(ofVec3f(x2, -h0, z2) + p, ofVec3f(x3, -h0, z3) + p));
		// cylinder
		mLines.push_back(Line::make(ofVec3f(x2, -h0, z2) + p, ofVec3f(x3, -h0 - h1, z3) + p));
	}

	mPoints.assign(mLines.size(), Point());
	reset();
}

void Water::setup(float w, float h, float d, int resX, int resZ)
{
	mW = w;
	mH = h;
	mD = d;
	mResX = resX;
	mResZ = resZ;
	for (auto j : rep(mResZ)) {
		for (auto i : rep(mResX)) {
			for (auto k : 4_i) {
				mLines.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
			}
		}
	}
    //for (auto i : rep(mResX)) {
    //    for (auto k : 2_i) {
    //        mLines.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
    //    }
    //}
    //for (auto j : rep(mResZ)) {
    //    for (auto k : 2_i) {
    //        mLines.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
    //    }
    //}
	mPoints.assign(mLines.size(), Point());
	reset();
}

float Water::displacement(int i, int j, float t)
{
	float y {mH};
	y += ::cosf(i * 0.3f + t * 0.98f) * 0.7f;
	y += ::cosf(i * 0.3f + t * 10.3f) * 0.6f;
	y += ::cosf(i * 0.3f + t * 97.6f) * 0.5f;
	y += ::sinf(j * 0.3f + t * 0.98f) * 0.7f;
	y += ::sinf(j * 0.3f + t * 10.3f) * 0.6f;
	y += ::sinf(j * 0.3f + t * 97.6f) * 0.5f;
	return y;
}

void Water::update()
{
	const float t {ofGetElapsedTimef() * 0.01f};
	for (auto j : rep(mResZ)) {
		for (auto i : rep(mResX)) {
			const float y0 {displacement(i, j, t)};
			const float y1 {displacement(i, j + 1, t)};
			const float y2 {displacement(i + 1, j + 1, t)};
			const float y3 {displacement(i + 1, j, t)};

			const float x0 {mW / (float) mResX * (float)i};
			const float z0 {mD / (float) mResZ * (float)j};
			const float x1 {mW / (float) mResX * (float)(i + 1)};
			const float z1 {mD / (float) mResZ * (float)(j + 1)};
			auto v0 = ofVec3f(x0, y0, z0);
			auto v1 = ofVec3f(x0, y1, z1);
			auto v2 = ofVec3f(x1, y2, z1);
			auto v3 = ofVec3f(x1, y3, z0);
			mLines.at(j * mResX * 4 + i * 4 + 0).set(v0, v1);
			mLines.at(j * mResX * 4 + i * 4 + 1).set(v1, v2);
			mLines.at(j * mResX * 4 + i * 4 + 2).set(v2, v3);
			mLines.at(j * mResX * 4 + i * 4 + 3).set(v3, v0);
		}
	}
    //for (auto i : rep(mResX)) {
    //    const float y0 {displacement(i, 0, t)};
    //    const float y1 {displacement(i, mResZ - 1, t)};
    //    const float x {mW / (float) mResX * (float)i};
    //    auto v0 = ofVec3f(x, y0, 0.f);
    //    auto v1 = ofVec3f(x, y1, mD);
    //    mLines.at(mResX * mResZ * 4 + i * 2 + 0).set(v0, ofVec3f(x, 0.f, 0.f));
    //    mLines.at(mResX * mResZ * 4 + i * 2 + 1).set(v1, ofVec3f(x, 0.f, mD));
    //}
    //for (auto j : rep(mResZ)) {
    //    const float y0 {displacement(0, j, t)};
    //    const float y1 {displacement(mResX - 1, j, t)};
    //    const float z {mD / (float) mResZ * (float)j};
    //    auto v0 = ofVec3f(0, y0, z);
    //    auto v1 = ofVec3f(mW, y1, z);
    //    mLines.at(mResX * mResZ * 4 + mResX * 2 + j * 2 + 0).set(v0, ofVec3f(0.f, 0.f, z));
    //    mLines.at(mResX * mResZ * 4 + mResX * 2 + j * 2 + 1).set(v1, ofVec3f(mW, 0.f, z));
    //}
}

void Water::draw()
{
    LineObj::draw();
    if (LineObj::enableAnimation) {
        return;
    }
    ScopedStyle s;
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    ofFill();
    ofSetColor(ofColor::white, 100);
    const float t {ofGetElapsedTimef() * 0.01f};
    for (auto i : rep(mResX)) {
        const float y0 {displacement(i + 0, mResZ - 1, t)};
        const float y1 {displacement(i + 1, mResZ - 1, t)};
        const float y2 {displacement(i + 0, 0, t)};
        const float y3 {displacement(i + 1, 0, t)};
        const float x0 {mW / (float) mResX * (float)(i + 0)};
        const float x1 {mW / (float) mResX * (float)(i + 1)};
        auto v0 = ofVec3f(x0, y0, mD);
        auto v1 = ofVec3f(x1, y1, mD);
        auto v2 = ofVec3f(x0, 0.f, mD);
        auto v3 = ofVec3f(x1, 0.f, mD);
        auto v4 = ofVec3f(x0, y2, 0.f);
        auto v5 = ofVec3f(x1, y3, 0.f);
        auto v6 = ofVec3f(x0, 0.f, 0.f);
        auto v7 = ofVec3f(x1, 0.f, 0.f);
        ofTriangle(v0, v1, v2);
        ofTriangle(v1, v3, v2);
        ofTriangle(v4, v5, v6);
        ofTriangle(v5, v7, v6);
    }
    for (auto j : rep(mResZ)) {
        const float y0 {displacement(mResX - 1, 0, t)};
        const float y1 {displacement(mResX - 1, 1, t)};
        const float y2 {displacement(0, j + 0, t)};
        const float y3 {displacement(0, j + 1, t)};
        const float z0 {mD / (float) mResZ * (float)(j + 0)};
        const float z1 {mD / (float) mResZ * (float)(j + 1)};
        auto v0 = ofVec3f(mW, y0, z0);
        auto v1 = ofVec3f(mW, y1, z1);
        auto v2 = ofVec3f(mW, 0.f, z0);
        auto v3 = ofVec3f(mW, 0.f, z1);
        auto v4 = ofVec3f(0.f, y2, z0);
        auto v5 = ofVec3f(0.f, y3, z1);
        auto v6 = ofVec3f(0.f, 0.f, z0);
        auto v7 = ofVec3f(0.f, 0.f, z1);
        ofTriangle(v0, v1, v2);
        ofTriangle(v1, v3, v2);
        ofTriangle(v4, v5, v6);
        ofTriangle(v5, v7, v6);
    }
    ofEnableDepthTest();
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
	const auto res = 10;
	mLegs.push_back(Cylinder::create(ofVec3f(legRad, legH * 0.5f, legRad), legRad, legH, res));
	mLegs.push_back(Cylinder::create(ofVec3f(w - legRad, legH * 0.5f, legRad), legRad, legH, res));
	mLegs.push_back(Cylinder::create(ofVec3f(legRad, legH * 0.5f, d - legRad), legRad,  legH, res));
	mLegs.push_back(Cylinder::create(ofVec3f(w - legRad, legH * 0.5f, d - legRad), legRad, legH, res));
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