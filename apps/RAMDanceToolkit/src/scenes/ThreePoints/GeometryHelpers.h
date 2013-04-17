// 
// GeometryHelpers.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// from the bourkster.
// https://gist.github.com/2724338
//--------------------------------------------------------------
bool lineLineIntersectSegment(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4, ofVec3f &pa, ofVec3f &pb) {
	float mua = 0;
	float mub = 0;
	
	ofVec3f p13,p43,p21;
	
	float d1343,d4321,d1321,d4343,d2121;
	float numer,denom;
	p13 = p1 - p3;
	p43 = p4 - p3;
	
	
	if (ABS(p43.x) < FLT_EPSILON && ABS(p43.y) < FLT_EPSILON && ABS(p43.z) < FLT_EPSILON)
		return false;
	
	p21 = p2 - p1;
	
	if (ABS(p21.x) < FLT_EPSILON && ABS(p21.y) < FLT_EPSILON && ABS(p21.z) < FLT_EPSILON)
		return false;
	
	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
	
	denom = d2121 * d4343 - d4321 * d4321;
	if (ABS(denom) < FLT_EPSILON)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;
	
	mua = numer / denom;
	mub = (d1343 + d4321 * mua) / d4343;
	
	pa = p1 + p21 * mua;
	pb = p3 + p43 * mub;
	
	return true;
}

// https://gist.github.com/2724338
//--------------------------------------------------------------
ofVec3f lineLineIntersection(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4) {
	ofVec3f pa, pb;
	lineLineIntersectSegment(p1,p2,p3,p4,pa,pb);
	return (pa+pb)/2.f;
}

//--------------------------------------------------------------
void findCircle(const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, ofVec3f& center, ofVec3f& normal, float& radius) {
	ofVec3f ab = a - b, cb = c - b;
	normal = ab.getCrossed(cb);
	ofVec3f abm = ab / 2, cbm = cb / 2;
	ofVec3f abo = abm.getRotated(90, normal), cbo = cbm.getRotated(90, normal);
	abm += b, cbm += b;
	center = lineLineIntersection(abm, abm + abo, cbm, cbm + cbo);
	radius = center.distance(b);
	normal.normalize();
}

//--------------------------------------------------------------
void findSphere(const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, const ofVec3f& d, ofVec3f& center, float& radius) {
	ofVec3f c1, c2, n1, n2;
	float r1, r2;
	findCircle(a, b, c, c1, n1, r1);
	findCircle(b, c, d, c2, n2, r2);
	center = lineLineIntersection(c1, c1 + n1, c2, c2 + n2);
	radius = center.distance(a);
}
