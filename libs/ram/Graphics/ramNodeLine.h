#pragma once

#include "ramMain.h"

#include "ramNodeIdentifer.h"
#include "ramNodeFinder.h"

class ramNodeLine
{
public:
	
	ramNodeFinder from;
	ramNodeFinder cn0;
	ramNodeFinder cn1;
	ramNodeFinder to;
	
	float getDistance()
	{
		ramNode from_n;
		ramNode to_n;
		
		if (!from.get(from_n)) return -1;
		if (!to.get(to_n)) return -1;
		
		ofVec3f p0 = from_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		
		return p0.distance(p1);
	}

	ofPolyline getLine()
	{
		ofPolyline result;
		
		ramNode from_n;
		ramNode to_n;
		
		if (!from.get(from_n)) return result;
		if (!to.get(to_n)) return result;
		
		ofVec3f p0 = from_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));

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
		
		if (!from.get(from_n)) return result;
		if (!to.get(to_n)) return result;
		if (!cn0.get(cn0_n)) return result;
		if (!cn1.get(cn1_n)) return result;
		
		ofVec3f p0 = from_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		
		if (p0.distance(p1) < 1000)
		{
			float gain = 100; // style["curve_gain"].as<float>();
			
			ofVec3f cp0 = cn0_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
			ofVec3f cp1 = cn1_n.getGlobalTransformMatrix().preMult(-ofVec3f(0, 0, 0));
			
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

