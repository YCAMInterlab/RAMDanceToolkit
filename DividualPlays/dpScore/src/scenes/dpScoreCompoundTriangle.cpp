//
//  dpScoreCompoundTriangle.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompoundTriangle.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

static bool _inited {false};
static ofVboMesh _triangle;

Triangle Triangle::make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2)
{
	Triangle t;
	t.set(_p0, _p1, _p2);
	return t;
}

Triangle Triangle::make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2,
                        const ofVec2f& _t0, const ofVec2f& _t1, const ofVec2f& _t2)
{
	Triangle t;
	t.set(_p0, _p1, _p2, _t0, _t1, _t2);
	return t;
}

void Triangle::set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2)
{
    if (!_inited) {
        _triangle.addVertex(ofVec3f::zero());
        _triangle.addVertex(ofVec3f::zero());
        _triangle.addVertex(ofVec3f::zero());
        _triangle.addTexCoord(ofVec2f::zero());
        _triangle.addTexCoord(ofVec2f::zero());
        _triangle.addTexCoord(ofVec2f::zero());
        _inited = true;
    }
    
	p0 = _p0;
	p1 = _p1;
	p2 = _p2;
	c = (p0 + p1 + p2) / 3.f;
}

void Triangle::set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2,
                   const ofVec2f& _t0, const ofVec2f& _t1, const ofVec2f& _t2)
{
	set(_p0, _p1, _p2);
	t0 = _t0;
	t1 = _t1;
	t2 = _t2;
}

void Triangle::draw()
{
    _triangle.getVertices()[0] = p0;
    _triangle.getVertices()[1] = p1;
    _triangle.getVertices()[2] = p2;
    _triangle.getTexCoords()[0] = t0;
    _triangle.getTexCoords()[1] = t1;
    _triangle.getTexCoords()[2] = t2;
    
    ScopedMatrix m;
    if (compoundGetEnableCollapse()) {
        transformGL();
    }
    _triangle.draw();
}

DP_SCORE_NAMESPACE_END