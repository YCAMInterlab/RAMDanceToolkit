//
//  dpScoreNodeDisplay.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreNodeDisplay.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"
#include "dpScoreObjects.h"
#include "dpScoreNodeStage.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

NodeDisplay::NodeDisplay()
{
	title = "Displays";
	titleJP = "ディスプレイ";
	descriptionJP = "RAMシステムはその情報を\n光や音や映像へと変換します";

	addAimingOffset(getFrontLeft() + ofVec3f(getFrontDisplayWidth() * 0.5f - 6.f,
	                                         getFrontDisplayHeight() * 0.5f,
	                                         getFrontDisplayDepth() * 0.5f + 15.f));

	addAimingOffset(getFrontRight() + ofVec3f(getFrontDisplayWidth() * 0.5f - 8.f,
	                                          getFrontDisplayHeight() * 0.5f,
	                                          getFrontDisplayDepth() * 0.5f - 30.f));

	addAimingOffset(getRearLeft() + ofVec3f(getRearDisplayWidth() * 0.5f - 10.f,
	                                        getRearDisplayHeight() * 0.5f,
	                                        getRearDisplayDepth() * 0.5f - 40.f));

	addAimingOffset(getRearRight() + ofVec3f(getRearDisplayWidth() * 0.5f + 10.f,
	                                         getRearDisplayHeight() * 0.5f,
	                                         getRearDisplayDepth() * 0.5f - 40.f));
	getCamera().setFov(35.f);
	getCamera().setPosition(-NodeStage::kWidth * 0.5f + 80.f, 255.f, 200.f);
	getCamera().setOrientation(ofVec3f(0.f, 150.f, 0.f));

	const int fboSize {4};
	fbo.allocate(getFrontDisplayWidth() * fboSize, getFrontDisplayHeight() * fboSize);

	{
		const float w {getRearDisplayWidth()};
		const float h {getRearDisplayHeight()};
		const float d {getRearDisplayDepth()};
		mRears.push_back(Box::create(ofVec3f::zero(), w, h, d));
		mRears.push_back(Box::create(ofVec3f::zero(), w, h, d));
	}
	{
		const float w {getFrontDisplayWidth()};
		const float h {getFrontDisplayHeight()};
		const float d {getFrontDisplayDepth()};
		mFronts.push_back(Box::create(ofVec3f::zero(), w, h, d));
		mFronts.push_back(Box::create(ofVec3f::zero(), w, h, d));
	}
}

NodeDisplay::~NodeDisplay()
{

}

void NodeDisplay::customDraw()
{
	ScopedStyle s;
	setStyle(*this);
	{
		auto drawFbo = [&]() {
				       if (compoundGetEnableCollapse()) return;
				       ScopedStyle s;
				       ofFill();
				       ofSetColor(ofColor::black);
				       ofRect(0.f, 0.f, getRearDisplayWidth(), getRearDisplayHeight());
				       ScopedTranslate t(0.f, 0.f, getRearDisplayDepth());
				       ofSetColor(ofColor::gray);
				       fbo.draw(0.f, 0.f, getRearDisplayWidth(), getRearDisplayHeight());
			       };

		const float w {getRearDisplayWidth()};
		const float h {getRearDisplayHeight()};
		const float d {getRearDisplayDepth()};
		{
			ScopedMatrix m;
			ofTranslate(getRearLeft());
			ofRotateY(30.f);
			ofRotateX(getRearDisplayAngle());
			mRears.at(0).draw();
			drawFbo();
		}
		{
			ScopedMatrix m;
			ofTranslate(getRearRight());
			ofTranslate(12.f, 0.f, -50.f);
			ofRotateY(-30.f);
			ofRotateX(getRearDisplayAngle());
			mRears.at(1).draw();
			drawFbo();
		}
	}
	{
		const float w {getFrontDisplayWidth()};
		const float h {getFrontDisplayHeight()};
		const float d {getFrontDisplayDepth()};
		const float wireOffset {20.f};

		auto drawFbo = [&]() {
				       if (compoundGetEnableCollapse()) return;
				       ScopedStyle s;
				       ofFill();
				       ofSetColor(ofColor::white);
				       fbo.draw(0.f, 0.f, getFrontDisplayWidth(), getFrontDisplayHeight());
				       ScopedTranslate t(0.f, 0.f, getFrontDisplayDepth());
				       ofSetColor(ofColor::black);
				       ofRect(0.f, 0.f, getFrontDisplayWidth(), getFrontDisplayHeight());
			       };

		{
			ScopedMatrix m;
			ofTranslate(getFrontLeft());
			ofRotateY(-getFrontDisplayAngle());
			ofLine(ofVec3f(w * 0.5f - wireOffset, h, d * 0.5f), ofVec3f(w * 0.5f - wireOffset, h, d * 0.5f));
			ofLine(ofVec3f(w * 0.5f + wireOffset, h, d * 0.5f), ofVec3f(w * 0.5f + wireOffset, h, d * 0.5f));
			mFronts.at(0).draw();
			drawFbo();
		}
		{
			ScopedMatrix m;
			ofTranslate(getFrontRight());
			ofRotateY(getFrontDisplayAngle());
			ofLine(ofVec3f(w * 0.5f - wireOffset, h, d * 0.5f), ofVec3f(w * 0.5f - wireOffset, h, d * 0.5f));
			ofLine(ofVec3f(w * 0.5f + wireOffset, h, d * 0.5f), ofVec3f(w * 0.5f + wireOffset, h, d * 0.5f));
			mFronts.at(1).draw();
			drawFbo();
		}
	}
}

ofVec3f NodeDisplay::getFrontLeft()
{
	return ofVec3f(-NodeStage::kWidth * 0.5f - 61.f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
}
ofVec3f NodeDisplay::getFrontRight()
{
	return ofVec3f(NodeStage::kWidth * 0.5f + 61.f, 214.f, NodeStage::kDepth * 0.5f - 50.f);
}
float NodeDisplay::getFrontDisplayWidth()
{
	return 93.f;
}
float NodeDisplay::getFrontDisplayHeight()
{
	return 63.f;
}
float NodeDisplay::getFrontDisplayDepth()
{
	return 7.f;
}
float NodeDisplay::getFrontDisplayAngle()
{
	return 30.f;
}

ofVec3f NodeDisplay::getRearLeft()
{
	return ofVec3f(-NodeStage::kWidth * 0.5f - 20.f, 0.f, -NodeStage::kDepth * 0.5f - 10.f);
}
ofVec3f NodeDisplay::getRearRight()
{
	return ofVec3f(NodeStage::kWidth * 0.5f - getRearDisplayWidth() + 20.f, 0.f, -NodeStage::kDepth * 0.5f - 10.f);
}
float NodeDisplay::getRearDisplayWidth()
{
	return 105.f;
}
float NodeDisplay::getRearDisplayHeight()
{
	return 61.f;
}
float NodeDisplay::getRearDisplayDepth()
{
	return 4.f;
}
float NodeDisplay::getRearDisplayAngle()
{
	return -30.f;
}

DP_SCORE_NAMESPACE_END