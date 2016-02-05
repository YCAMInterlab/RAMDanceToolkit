//
//  dpScoreTrianglePrimitives.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreTrianglePrimitives.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

TriRect TriRect::create(const ofVec3f& p, float w, float h, int resX, int resY)
{
	TriRect r;
	r.setup(p, w, h, resX, resY);
	return r;
}

void TriRect::setup(const ofVec3f& p, float w, float h, int resX, int resY)
{
	resX = std::max(resX, 1);
	resY = std::max(resY, 1);

	mTypes.clear();
	const float sx {w / (float)resX};
	const float sy {h / (float)resY};
	const float tsx {1.f / (float)resX};
	const float tsy {1.f / (float)resY};
	for (auto j : rep(resY)) {
		for (auto i : rep(resX)) {
			const float x0 {-w * 0.5f + sx * (float)i};
			const float y0 {-h * 0.5f + sy * (float)j};
			const float x1 {-w * 0.5f + sx * (float)(i + 1)};
			const float y1 {-h * 0.5f + sy * (float)(j + 1)};
			const float tx0 {tsx * (float)i};
			const float ty0 {1.f - tsy * (float)j};
			const float tx1 {tsx * (float)(i + 1)};
			const float ty1 {1.f - tsy * (float)(j + 1)};

			const ofVec3f v0 {ofVec3f(x0, y0, 0.f) + p};
			const ofVec3f v1 {ofVec3f(x1, y0, 0.f) + p};
			const ofVec3f v2 {ofVec3f(x1, y1, 0.f) + p};
			const ofVec3f v3 {ofVec3f(x0, y1, 0.f) + p};
			const ofVec2f t0 {ofVec2f(tx0, ty0)};
			const ofVec2f t1 {ofVec2f(tx1, ty0)};
			const ofVec2f t2 {ofVec2f(tx1, ty1)};
			const ofVec2f t3 {ofVec2f(tx0, ty1)};

			mTypes.push_back(Triangle::make(v0, v1, v2, t0, t1, t2));
			mTypes.push_back(Triangle::make(v2, v3, v0, t2, t3, t0));
		}
	}
	reset();
}

DP_SCORE_NAMESPACE_END