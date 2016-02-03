//
//  dpScoreGadgets.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreGadgets.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

Desk::Desk()
{
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

Chair::Chair()
{
	mBoxes.push_back(Box::create(ofVec3f::zero(), 4.f, 1.5f, 35.f));
	mBoxes.push_back(Box::create(ofVec3f(42.f - 4.f, 0.f, 0.f), 4.f, 1.5f, 35.f));
	mBoxes.push_back(Box::create(ofVec3f(0.f, 0.f, 35.f - 1.5f), 4.f, 34.f, 1.5f));
	mBoxes.push_back(Box::create(ofVec3f(42.f - 4.f, 0.f, 35.f - 1.5f), 4.f, 34.f, 1.5f));
	mBoxes.push_back(Box::create(ofVec3f::zero(), 42.f, 5.f, 35.f));
	mBoxes.push_back(Box::create(ofVec3f::zero(), -4.f, 1.5f, -32.f));
	mBoxes.push_back(Box::create(ofVec3f(42.f, 0.f, 0.f), 4.f, 1.5f, -32.f));
	mBoxes.push_back(Box::create(ofVec3f::zero(), 42.f, 53.f, 5.f));
}

void Chair::draw()
{
	ScopedStyle s;
	for (auto i : 4_i) {
		mBoxes.at(i).draw();
	}

	{
		ScopedMatrix m;
		ofTranslate(0.f, 34.f, 0.f);
		mBoxes.at(4).draw();
		ofTranslate(0.f, 0.f, 35.f);

		{
			ScopedTranslate t(0.f, 22.f, 0.f);
			mBoxes.at(5).draw();
			mBoxes.at(6).draw();
		}

		ofTranslate(0.f, 5.f, 0.f);
		ofRotateX(10.f);
		mBoxes.at(7).draw();
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

	if (!CompoundLine::enableAnimation && drawDisplay && angle >= 15.f) {
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