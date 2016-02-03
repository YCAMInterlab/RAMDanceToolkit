//
//  dpScoreFlowChartRDTK.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreFlowChartRDTK.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

void FlowChartRDTK::draw(SceneFlowChart::SkeletonVec& skeletons)
{
	ofEnableDepthTest();
	drawFloor();

	for (auto skl : skeletons) {
		for (auto& n : skl->getJoints()) {
			ofSetColor(ofColor::magenta);
			n.draw();
			if (n.getParent()) {
				ofLine(n.getGlobalPosition(), n.getParent()->getGlobalPosition());
			}
			ofNode n2 = n;
			n2.setTransformMatrix(n.getGlobalTransformMatrix() * n2.getGlobalTransformMatrix());
			{
				ScopedMatrix m;
				ofMultMatrix(n2.getGlobalTransformMatrix());
				ofSetColor(ofColor::cyan);
				ofDrawBox(ofVec3f::zero(), 10.f);
			}
			ofSetColor(ofColor::white);
			ofLine(n.getGlobalPosition(), n2.getGlobalPosition());
		}
	}
}

void FlowChartRDTK::drawFloor()
{
	ofSetRectMode(OF_RECTMODE_CENTER);
	{
		const float d {1000.f};
		const int r {10};
		const float s {d / (float)r};
		{
			ScopedTranslate t(0.f, -1.f, 0.f);
			ScopedRotateX rx(90.f);
			ofFill();
			ofSetColor(32);
			ofRect(ofVec3f::zero(), d, d);
		}
		ScopedRotateX rx(90.f);
		ofNoFill();
		ofSetColor(ofColor::white);
		ofRect(ofVec3f::zero(), d, d);
		for (auto i : rep(r)) {
			for (auto j : rep(r)) {
				ofLine(-d * 0.5f + i * s, -d * 0.5f + j * s, -d * 0.5f + (i + 1) * s, -d * 0.5f + j * s);
				ofLine(-d * 0.5f + i * s, -d * 0.5f + j * s, -d * 0.5f + i * s, -d * 0.5f + (j + 1) * s);
			}
		}
	}
}

DP_SCORE_NAMESPACE_END