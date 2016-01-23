
//
//  dpScoreNodeStage.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/19/16.
//
//

#include "dpScoreNodeStage.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

NodeStage::NodeStage()
{
	const int n {6};
	vector<ofVec3f> vertices;
	vector<ofIndexType> indices;
	for (auto i : rep(n)) {
		for (auto j : rep(n)) {
			const float x {-kWidth * 0.5f};
			const float z {-kDepth * 0.5f};
			const float xs {kWidth / (float)(n - 1)};
			const float zs {kDepth / (float)(n - 1)};
			vertices.push_back(ofVec3f(x + xs * (float)i, 0.f, z + zs * (float)j));
		}
	}
	for (auto i : rep(n - 1)) {
		for (auto j : rep(n - 1)) {
			indices.push_back(i * n + j);
			indices.push_back(i * n + j + 1);

			indices.push_back(i * n + j);
			indices.push_back((i + 1) * n + j);

			indices.push_back((i + 1) * n + j);
			indices.push_back((i + 1) * n + j + 1);

			indices.push_back((i + 1) * n + j + 1);
			indices.push_back(i * n + j + 1);
		}
	}

	mFloor.setVertexData(&vertices.front(), vertices.size(), GL_STATIC_DRAW);
	mFloor.setIndexData(&indices.front(), indices.size(), GL_STATIC_DRAW);

	fbo.allocate(kWidth, kScrH, GL_RGBA16);
}

NodeStage::~NodeStage()
{

}

void NodeStage::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
	ofSetRectMode(OF_RECTMODE_CENTER);

	if (!forFbo) {
		ScopedStyle s;
		ScopedTranslate t(0.f, kHeight - kScrH, -kDepth * 0.5f - 300.f);
		ofFill();
		ofSetColor(128);
		fbo.draw(0.f, 0.f, kWidth, kScrH);

	}
	ofRect(ofVec3f(0.f, kHeight - kScrH, -kDepth * 0.5f - 300.f), kWidth, kScrH);

	{
		ScopedStyle s;
		ScopedRotateX rx(90.f);
		ofFill();
		ofSetColor(32);
		ofRect(ofVec3f::zero(), kWidth, kDepth);
	}

	//mFloor.draw(GL_POINTS, 0, mFloor.getNumVertices());
	mFloor.drawElements(GL_LINES, mFloor.getNumIndices());
}

DP_SCORE_NAMESPACE_END