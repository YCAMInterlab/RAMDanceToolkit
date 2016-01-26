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

inline void setColor(const BaseNode& n)
{
	ofSetColor(128 + 127 * n.t);
}

inline void setStyle(const BaseNode& n)
{
	ofSetLineWidth(kFlowChartLineWidth);
	setColor(n);
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

struct Line {
	static Line make(const ofVec3f& p0, const ofVec3f& p1)
	{
		Line line;
		line.c = p0 * 0.5f + p1 * 0.5f;
		line.v = p1 - p0;
		return line;
	}
	void draw()
	{
		ofLine(c - v * 0.5f, c + v * 0.5f);
	}

	ofVec3f c;
	ofVec3f v;
};

class LineObj {
public:
	static bool enableAnimation;

	virtual ~LineObj() {
	}
    virtual void draw();

protected:
	void update();
	void reset();

	struct Point {
		ofVec3f rot;
		ofVec3f spd;
		ofVec3f ang;
		ofVec3f pos;
	};

	vector<Line> mLines;
	vector<Point> mPoints;
	bool mUpdate;
};

class Box: public LineObj {
public:
	static Box create(const ofVec3f& p, float w, float h, float d)
	{
		Box b;
		b.setup(p, w, h, d);
		return b;
	}

	void setup(const ofVec3f& p, float w, float h, float d);
};

class Rect : public LineObj {
public:
	static Rect create(const ofVec3f& p, float w, float h)
	{
		Rect r;
		r.setup(p, w, h);
		return r;
	}

	void setup(const ofVec3f& p, float w, float h);
};

inline void drawLine(const ofVec3f& c, const ofVec3f& v)
{
	ofLine(c - v * 0.5f, c + v * 0.5f);
}

inline void drawBox(const ofVec3f& p, float w, float h, float d)
{
	ofDrawBox(p.x + w * 0.5f, p.y + h * 0.5f, p.z + d * 0.5f, w, h, d);
}

struct Desk {
	Desk();
	void draw();

	static float getDimension()
	{
		return 91.f;
	}
	static float getHeight()
	{
		return 84.f;
	}
private:
	vector<Box> mBoxes;
};

struct Deck {
	Deck();
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
private:
	Box mTop;
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
	typedef void (*DrawDisplayFunc)(float w, float h);

	MacBook();
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

	DrawDisplayFunc drawDisplay {nullptr};

private:
	Box mBottom;
	Box mTop;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreObjects_h */
