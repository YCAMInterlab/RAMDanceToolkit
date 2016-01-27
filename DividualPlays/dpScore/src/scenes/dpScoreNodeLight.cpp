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
	title = "Lights";
	titleJP = "照明";
	descriptionJP = "照明";

	setGlobalPosition(0.f, getHeight(), 0.f);
	addAimingOffset(ofVec3f::zero());

	getCamera().setFov(50.f);
	getCamera().setPosition(0.f, 600.f, 500.f);
	getCamera().setOrientation(ofVec3f(15.f, 0.f, 0.f));

	for (auto j : rep(getNumZ())) {
		for (auto i : rep(getNumX())) {
			const float w {600.f};
			ParLight p;
			p.position = ofVec3f(-w * 0.5f + (w / (float)getNumX()) * i,
                                 0.f
                                 , -w * 0.5f + (w / (float)getNumZ()) * j);
			mParLights.push_back(p);
		}
	}
}

NodeLight::~NodeLight()
{

}

void NodeLight::customDraw()
{
	ScopedStyle s;
	setStyle(*this);

	for (auto i : rep(mParLights.size())) {
		auto& p = mParLights.at(i);
		p.angle = ::cos(getElapsedTime() + i * 0.5) * 45.f;
		p.draw();
	}
}

NodeLight::ParLight::ParLight()
{
	top.setup(ofVec3f(0.f, 0.f, 0.f), 25.f, 1.f, 3.f);
	left.setup(ofVec3f(0.f, 0.f, 0.f), 1.f, -24.f, 3.f);
	right.setup(ofVec3f(24.f, 0.f, 0.f), 1.f, -24.f, 3.f);

	body.setup(ofVec3f(0.f, -6.f, 0.f), 9.f, 28.f);
}

void NodeLight::ParLight::draw()
{
	ScopedStyle s;
    ScopedTranslate t(position);
    
	{
		ScopedTranslate t(-12.5f, 0.f, 0.f);
		top.draw();
		left.draw();
		right.draw();
	}
	{
		ofSetCylinderResolution(12, 1);
		ofSetConeResolution(12, 1);

		ScopedMatrix m;
		ofTranslate(0.f, -24.f);
		ofRotateX(angle);
        if (LineObj::enableAnimation == false) {
            ofDrawCone(ofVec3f(0.f, 15.f, 0.f), 9.f, -14.f);
        }
		body.draw();
	}
}

DP_SCORE_NAMESPACE_END