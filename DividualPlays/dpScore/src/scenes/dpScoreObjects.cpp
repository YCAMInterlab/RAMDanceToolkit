//
//  dpScoreObjects.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#include "dpScoreObjects.h"
#include "dpScoreScoped.h"

DP_SCORE_NAMESPACE_BEGIN

void Desk::draw()
{
	const float tableDim {getDimension()};
	const float tableH {getHeight()};
	const float tableThickness {11.f};
	const float legDim {5.f};
	drawBox(ofVec3f(0.f, tableH - tableThickness, 0.f), tableDim, tableThickness, tableDim);
	drawBox(ofVec3f(0.f, 0.f, 0.f), legDim, tableH - tableThickness, legDim);
	drawBox(ofVec3f(tableDim - legDim, 0.f, 0.f), legDim, tableH - tableThickness, legDim);
	drawBox(ofVec3f(tableDim - legDim, 0.f, tableDim - legDim), legDim, tableH - tableThickness, legDim);
	drawBox(ofVec3f(0.f, 0.f, tableDim - legDim), legDim, tableH - tableThickness, legDim);
}

void Deck::draw()
{
    ScopedStyle s;
    ofSetCylinderResolution(8, 1);
	const float w {getWidth()};
	const float d {getDepth()};
	const float h {getHeight()};
	const float thickness {11.f};
	const float legRad {2.5f};
    const float legH{h - thickness};
	drawBox(ofVec3f(0.f, h - thickness, 0.f), w, thickness, d);
	ofDrawCylinder(ofVec3f(legRad, legH * 0.5f, legRad), legRad, legH);
	ofDrawCylinder(ofVec3f(w - legRad, legH * 0.5f, legRad), legRad, legH);
	ofDrawCylinder(ofVec3f(legRad, legH * 0.5f, d - legRad), legRad,  legH);
	ofDrawCylinder(ofVec3f(w - legRad, legH * 0.5f, d - legRad), legRad, legH);
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

void MacBook::draw()
{
	ScopedStyle s;
	drawBox(ofVec3f::zero(), getWidth(), 1.f, getDepth());
	ScopedMatrix m;
	ofTranslate(0.f, 0.f, getDepth());
	ofRotateX(angle);
	drawBox(ofVec3f::zero(), getWidth(), 0.5f, -getDepth());
}

DP_SCORE_NAMESPACE_END