#pragma once

#include "ramMain.h"

#include "ramNodeIdentifer.h"
#include "ramNodeFinder.h"

class ramNodeLine
{
public:
	
	ramNodeFinder from;
	ramNodeFinder control0;
	ramNodeFinder control1;
	ramNodeFinder to;
	
	ramNodeLine() {}
	
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

	ramNodeLine& line()
	{
		polyline.clear();
		
		ramNode from_n;
		ramNode to_n;
		
		if (!from.get(from_n)) return *this;
		if (!to.get(to_n)) return *this;
		
		ofVec3f p0 = from_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));

		if (p0.distance(p1) < 1000)
		{
			polyline.addVertex(p0);
			polyline.addVertex(p1);
		}
		
		return *this;
	}
	
	ramNodeLine& curve(float strength = 100)
	{
		polyline.clear();
		
		ramNode from_n;
		ramNode control0_n;
		ramNode control1_n;
		ramNode to_n;
		
		if (!from.get(from_n)) return *this;
		if (!to.get(to_n)) return *this;
		if (!control0.get(control0_n)) return *this;
		if (!control1.get(control1_n)) return *this;
		
		ofVec3f p0 = from_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		ofVec3f p1 = to_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
		
		if (p0.distance(p1) < 1000)
		{
			ofVec3f cp0 = control0_n.getGlobalTransformMatrix().preMult(ofVec3f(0, 0, 0));
			ofVec3f cp1 = control1_n.getGlobalTransformMatrix().preMult(-ofVec3f(0, 0, 0));
			
			ofVec3f pp0 = (p0 - cp0).normalized() * strength;
			ofVec3f pp1 = (p1 - cp1).normalized() * strength;
			
			pp0 += p0;
			pp1 += p1;
			
			polyline.addVertex(p0);
			polyline.bezierTo(pp0, pp1, p1, 64);
		}
		
		return *this;
	}
	
	ramNodeLine& resampling(float spacing = 5)
	{
		spacing = ofClamp(spacing, 0.1, 100);
		polyline = polyline.getResampledBySpacing(spacing);
		return *this;
	}
	
	ramNodeLine& spiral(float radius = 10, float rotate = 10)
	{
		ofPolyline pp;
		pp.resize(polyline.size());
		
		ofVec3f axis;
		
		float freq = (1. / polyline.size()) * rotate;
		float phase = 0;
		
		for (int i = 0; i < polyline.size(); i++)
		{
			float t = ofMap(i, 0, polyline.size() - 1, 0, 1);
			
			ofVec3f &v0 = polyline[i];
			ofVec3f &v1 = polyline[i + 1];
			
			ofVec3f d = (v1 - v0);

			if (i == 0)
			{
				axis = d.crossed(ofVec3f(0, 1, 0)).normalized();
			}
			else
			{
				axis.rotateRad(freq * TWO_PI, d);
			}
			
			float s = sin(t * PI);
			pp[i] = v0 + axis * radius * s;
			
			phase += freq;
		}
		
		polyline = pp;
		return *this;
	}
	
	void draw()
	{
		polyline.draw();
	}
	
	void drawDotted(int spaceing = 5)
	{
		if (polyline.size() > 0)
		{
			glBegin(GL_LINES);
			for (int i = 0; i < polyline.size() - 1; i += spaceing)
			{
				ofVec3f &v0 = polyline[i];
				ofVec3f &v1 = polyline[i + 1];
				glVertex3fv(v0.getPtr());
				glVertex3fv(v1.getPtr());
			}
			glEnd();
		}
	}
	
private:
	
	ofPolyline polyline;
};

