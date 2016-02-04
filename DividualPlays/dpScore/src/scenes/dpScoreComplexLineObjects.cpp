//
//  dpScoreComplexLineObjects.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreComplexLineObjects.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

Speaker Speaker::create(const ofVec3f& p)
{
	Speaker s;
	s.setup(p);
	return s;
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


	mTypes.clear();
	mTypes.push_back(Line::make(v0, v1));
	mTypes.push_back(Line::make(v1, v2));
	mTypes.push_back(Line::make(v2, v3));
	mTypes.push_back(Line::make(v3, v0));

	mTypes.push_back(Line::make(v0, v4));
	mTypes.push_back(Line::make(v1, v5));
	mTypes.push_back(Line::make(v2, v6));
	mTypes.push_back(Line::make(v3, v7));

	mTypes.push_back(Line::make(v4, v5));
	mTypes.push_back(Line::make(v5, v6));
	mTypes.push_back(Line::make(v6, v7));
	mTypes.push_back(Line::make(v7, v4));

	mTypes.push_back(Line::make(v8, v9));
	mTypes.push_back(Line::make(v9, v10));
	mTypes.push_back(Line::make(v10, v11));
	mTypes.push_back(Line::make(v11, v8));

	mTypes.push_back(Line::make(v7, v8));
	mTypes.push_back(Line::make(v3, v9));
	mTypes.push_back(Line::make(v2, v10));
	mTypes.push_back(Line::make(v6, v11));

	mPoints.assign(mTypes.size(), Point());
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

Funnel Funnel::create(const ofVec3f& p, float r0, float r1, float h0, float h1)
{
	Funnel f;
	f.setup(p, r0, r1, h0, h1);
	return f;
}

void Funnel::setup(const ofVec3f& p, float r0, float r1, float h0, float h1)
{
	mTypes.clear();
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
		mTypes.push_back(Line::make(ofVec3f(x0, 0.f, z0) + p, ofVec3f(x1, 0.f, z1) + p));
		// side face
		mTypes.push_back(Line::make(ofVec3f(x0, 0.f, z0) + p, ofVec3f(x2, -h0, z2) + p));
		// bottom circle
		//mTypes.push_back(Line::make(ofVec3f(x2, -h0, z2) + p, ofVec3f(x3, -h0, z3) + p));
		// cylinder
		mTypes.push_back(Line::make(ofVec3f(x2, -h0, z2) + p, ofVec3f(x3, -h0 - h1, z3) + p));
	}

	mPoints.assign(mTypes.size(), Point());
	reset();
}

Water Water::create(float w, float h, float d, int resX, int resZ)
{
	Water water;
	water.setup(w, h, d, resX, resZ);
	return water;
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
				mTypes.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
			}
		}
	}
	//for (auto i : rep(mResX)) {
	//    for (auto k : 2_i) {
	//        mTypes.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
	//    }
	//}
	//for (auto j : rep(mResZ)) {
	//    for (auto k : 2_i) {
	//        mTypes.push_back(Line::make(ofVec3f::zero(), ofVec3f::zero()));
	//    }
	//}
	mPoints.assign(mTypes.size(), Point());
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
			mTypes.at(j * mResX * 4 + i * 4 + 0).set(v0, v1);
			mTypes.at(j * mResX * 4 + i * 4 + 1).set(v1, v2);
			mTypes.at(j * mResX * 4 + i * 4 + 2).set(v2, v3);
			mTypes.at(j * mResX * 4 + i * 4 + 3).set(v3, v0);
		}
	}
	//for (auto i : rep(mResX)) {
	//    const float y0 {displacement(i, 0, t)};
	//    const float y1 {displacement(i, mResZ - 1, t)};
	//    const float x {mW / (float) mResX * (float)i};
	//    auto v0 = ofVec3f(x, y0, 0.f);
	//    auto v1 = ofVec3f(x, y1, mD);
	//    mTypes.at(mResX * mResZ * 4 + i * 2 + 0).set(v0, ofVec3f(x, 0.f, 0.f));
	//    mTypes.at(mResX * mResZ * 4 + i * 2 + 1).set(v1, ofVec3f(x, 0.f, mD));
	//}
	//for (auto j : rep(mResZ)) {
	//    const float y0 {displacement(0, j, t)};
	//    const float y1 {displacement(mResX - 1, j, t)};
	//    const float z {mD / (float) mResZ * (float)j};
	//    auto v0 = ofVec3f(0, y0, z);
	//    auto v1 = ofVec3f(mW, y1, z);
	//    mTypes.at(mResX * mResZ * 4 + mResX * 2 + j * 2 + 0).set(v0, ofVec3f(0.f, 0.f, z));
	//    mTypes.at(mResX * mResZ * 4 + mResX * 2 + j * 2 + 1).set(v1, ofVec3f(mW, 0.f, z));
	//}
}

void Water::draw()
{
	CompoundLine::draw();
	if (compoundGetEnableCollapse()) {
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
		const float y0 {displacement(mResX - 1, j + 0, t)};
		const float y1 {displacement(mResX - 1, j + 1, t)};
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

DP_SCORE_NAMESPACE_END