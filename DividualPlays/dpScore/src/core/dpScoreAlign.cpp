//
//  dpScoreAlign.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#include "dpScoreAlign.h"

DP_SCORE_NAMESPACE_BEGIN

float aligned(float f)
{
	return ::floorf(f);
}

void alignedLine(const ofPoint& p1, const ofPoint& p2)
{
	alignedLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

void alignedLine(float x1, float y1, float x2, float y2)
{
	alignedLine(x1, y1, 0.0f, x2, y2, 0.0f);
}

void alignedLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	ofLine(aligned(x1), aligned(y1), aligned(z1), aligned(x2), aligned(y2), aligned(z2));
}

void alignedRect(const ofRectangle& r)
{
	alignedRect(r.x, r.y, 0.0f, r.width, r.height);
}

void alignedRect(const ofPoint& p, float w, float h)
{
	alignedRect(p.x, p.y, p.z, w, h);
}

void alignedRect(float x, float y, float w, float h)
{
	alignedRect(x, y, 0.0f, w, h);
}


void alignedRect(float x, float y, float z, float w, float h)
{
	ofRect(aligned(x), aligned(y), aligned(z), aligned(w), aligned(h));
}

void alignedTranslate(const ofPoint& p)
{
	alignedTranslate(p.x, p.y, p.z);
}

void alignedTranslate(float x, float y, float z)
{
	ofTranslate(aligned(x), aligned(y), aligned(z));
}

ofVec2f alignedVec2f(const ofVec2f& p)
{
	return ofVec2f(aligned(p.x), aligned(p.y));
}

ofVec2f alignedVec2f(float x, float y)
{
	return ofVec2f(aligned(x), aligned(y));
}

ofVec3f alignedVec3f(const ofVec3f& p)
{
	return ofVec3f(aligned(p.x), aligned(p.y), aligned(p.z));
}

ofVec3f alignedVec3f(float x, float y, float z)
{
	return ofVec3f(aligned(x), aligned(y), aligned(z));
}

ofRectangle alignedRectangle(float px, float py, float w, float h)
{
	return ofRectangle(aligned(px), aligned(py), aligned(w), aligned(h));
}

ofRectangle alignedRectangle(const ofPoint& p, float w, float h)
{
	return ofRectangle(alignedVec3f(p), aligned(w), aligned(h));
}

void beginAlign()
{
	ofPushMatrix();
	ofTranslate(0.5f, 0.5f, 0.f);
}

void endAlign()
{
	ofPopMatrix();
}

DP_SCORE_NAMESPACE_END