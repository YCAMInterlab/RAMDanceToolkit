//
//  dpScoreNodeLight.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#include "dpScoreNodeLight.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreNodeHakoniwa.h"

DP_SCORE_NAMESPACE_BEGIN

NodeLight::NodeLight()
{

}

NodeLight::~NodeLight()
{

}

void NodeLight::customDraw()
{
	ScopedStyle s;
	setStyle();

	ScopedTranslate t(0.f, getHeight(), 0.f);
    const int nX{8}, nZ{3};
    for (auto j : rep(nZ)) {
        for (auto i : rep(nX)) {
            const float w{600.f};
            ScopedTranslate t(-w * 0.5f + (w/(float)nX) * i, 0.f, -w * 0.5f + (w/(float)nZ) * j);
            drawParLight(::cos(ofGetElapsedTimef() + i * 0.5) * 45.f);
        }
    }
}

void NodeLight::drawParLight(float angle)
{
	ofSetCylinderResolution(12, 1);
	ofSetConeResolution(12, 1);
	{
		ScopedTranslate t(-12.5f, 0.f, 0.f);
		drawBox(ofVec3f(0.f, 0.f, 0.f), 25.f, 1.f, 3.f);
		drawBox(ofVec3f(0.f, 0.f, 0.f), 1.f, -24.f, 3.f);
		drawBox(ofVec3f(24.f, 0.f, 0.f), 1.f, -24.f, 3.f);
	}

	{
		ScopedMatrix m;
		ofTranslate(0.f, -24.f);
		//ofRotateX(::cos(ofGetElapsedTimef()) * 45.f);
		ofRotateX(angle);
		ofDrawCone(ofVec3f(0.f, 15.f, 0.f), 9.f, -14.f);
		ofDrawCylinder(ofVec3f(0.f, -6.f, 0.f), 9.f, 28.f);
	}
}

DP_SCORE_NAMESPACE_END