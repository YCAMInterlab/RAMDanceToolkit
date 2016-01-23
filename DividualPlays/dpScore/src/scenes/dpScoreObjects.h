//
//  dpScoreObjects.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreObjects_h
#define dpScoreObjects_h

#include "dpScoreBaseNode.h"

DP_SCORE_NAMESPACE_BEGIN

inline void setStyle(const BaseNode& n)
{
	ofSetLineWidth(kFlowChartLineWidth);
	ofSetColor(128 + 127 * n.t);
	ofNoFill();
	ofEnableDepthTest();
}

inline void setStyle()
{
    ofSetLineWidth(kFlowChartLineWidth);
    ofSetColor(ofColor::white);
    ofNoFill();
    ofEnableDepthTest();
}

inline void drawBox(const ofVec3f& p, float w, float h, float d)
{
	ofDrawBox(p.x + w * 0.5f, p.y + h * 0.5f, p.z + d * 0.5f, w, h, d);
}

struct Desk {
	void draw();
	static float getDimension()
	{
		return 91.f;
	}
	static float getHeight()
	{
		return 84.f;
	}
};

struct Deck {
	void draw();
	static float getWidth()
	{
		return 90.f;
	}
	static float getDepth()
	{
		return 180.f;
	}
	static float getHeight()
	{
		return 70.f;
	}
};

struct Chair {
	void draw();
	static float getWidth()
	{
		return 46.f;
	}
	static float getDepth()
	{
		return 35.f;
	}
};

struct MacBook {
	void draw();
	static float getWidth()
	{
		return 35.5f;
	}
	static float getDepth()
	{
		return 24.5f;
	}
	float angle {110.f};
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreObjects_h */
