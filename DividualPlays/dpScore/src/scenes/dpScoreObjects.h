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

// all object will make from OpenGL coord(righthanded)
struct Line {
	static Line make(const ofVec3f& p0, const ofVec3f& p1)
	{
		Line line;
		line.c = p0 * 0.5f + p1 * 0.5f;
		line.v = p1 - p0;
		return line;
	}
    
    void set(const ofVec3f& p0, const ofVec3f& p1)
    {
        c = p0 * 0.5f + p1 * 0.5f;
        v = p1 - p0;
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

class Speaker: public LineObj {
public:
    static Speaker create(const ofVec3f& p)
    {
        Speaker s;
        s.setup(p);
        return s;
    }
    
    void setup(const ofVec3f& p);
    
    static float getWidth();
    static float getHeight();
    static float getDepth();
};

class Cylinder: public LineObj {
public:
    static Cylinder create(const ofVec3f& p, float r, float h, int res = 16)
    {
        Cylinder c;
        c.setup(p, r, h, res);
        return c;
    }
    void setup(const ofVec3f& p, float r, float h, int res = 16);
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

// this object will make from center
class Funnel : public LineObj {
public:
    static Funnel create(const ofVec3f& p, float r0, float r1, float h0, float h1)
    {
        Funnel f;
        f.setup(p, r0, r1, h0, h1);
        return f;
    }
    void setup(const ofVec3f& p, float r0, float r1, float h0, float h2);
};

class Water : public LineObj {
public:
    static Water create(float w, float h, float d, int resX, int resZ)
    {
        Water water;
        water.setup(w, h, d, resX, resZ);
        return water;
    }
    void setup(float w, float h, float d, int resX, int resZ);
    void update();
    void draw() override;
    
private:
    float displacement(int i, int j, float t);
    float mW, mH, mD;
    int mResX, mResZ;
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

    //static float getDimension()
    //{
    //    return 60.f;
    //}
    static float getWidth()
    {
        return 60.f;
    }
    
    static float getDepth()
    {
        return 180.f;
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
		return 182.f;
	}
	static float getHeight()
	{
		return 84.f;
	}
private:
	Box mTop;
    vector<Cylinder> mLegs;
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
