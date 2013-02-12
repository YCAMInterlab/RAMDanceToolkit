#pragma once

#include "ramMain.h"

#include "ramNodeIdentifer.h"
#include "ramNodeFinder.h"

class ramNodeLine
{
public:
	
	ramNodeIdentifer from;
	ramNodeIdentifer cn0;
	ramNodeIdentifer cn1;
	ramNodeIdentifer to;

	ofPolyline getLine()
	{
		ofPolyline result;
		
		ramNode from_n;
		ramNode to_n;
		
		if (!ramNodeFinder::findNode(from, from_n)) return result;
		if (!ramNodeFinder::findNode(to, to_n)) return result;
		
		ofVec3f p0 = from_n.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getTransformMatrix().preMult(ofVec3f(0, 0, 0));

		if (p0.distance(p1) < 1000)
		{
			result.addVertex(p0);
			result.addVertex(p1);
		}
		
		return result;
	}
	
	ofPolyline getCurve()
	{
		ofPolyline result;
		
		ramNode from_n;
		ramNode cn0_n;
		ramNode cn1_n;
		ramNode to_n;
		
		if (!ramNodeFinder::findNode(from, from_n)) return result;
		if (!ramNodeFinder::findNode(cn0, cn0_n)) return result;
		if (!ramNodeFinder::findNode(cn1, cn1_n)) return result;
		if (!ramNodeFinder::findNode(to, to_n)) return result;
		
		ofVec3f p0 = from_n.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
		
		if (p0.distance(p1) < 1000)
		{
			float gain = 100; // style["curve_gain"].as<float>();
			
			ofVec3f cp0 = cn0_n.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
			ofVec3f cp1 = cn1_n.getTransformMatrix().preMult(-ofVec3f(0, 0, 0));
			
			ofVec3f pp0 = (p0 - cp0).normalized() * gain;
			ofVec3f pp1 = (p1 - cp1).normalized() * gain;
			
			pp0 += p0;
			pp1 += p1;
			
			result.addVertex(p0);
			result.bezierTo(pp0, pp1, p1, 64);
		}
		
		return result;
	}
};

