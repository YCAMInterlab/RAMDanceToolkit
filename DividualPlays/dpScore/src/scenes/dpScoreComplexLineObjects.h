//
//  dpScoreComplexLineObjects.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreComplexLineObjects_h
#define dpScoreComplexLineObjects_h

#include "dpScoreCompoundLine.h"

DP_SCORE_NAMESPACE_BEGIN

class Speaker: public CompoundLine {
public:
	static Speaker create(const ofVec3f& p);
	void setup(const ofVec3f& p);

	static float getWidth();
	static float getHeight();
	static float getDepth();
};

#pragma mark ___________________________________________________________________

// this object will make from center
class Funnel: public CompoundLine {
public:
	static Funnel create(const ofVec3f& p, float r0, float r1, float h0, float h1);
	void setup(const ofVec3f& p, float r0, float r1, float h0, float h2);
};

#pragma mark ___________________________________________________________________

class Water: public CompoundLine {
public:
	static Water create(float w, float h, float d, int resX, int resZ);
	void setup(float w, float h, float d, int resX, int resZ);
	void update();
	void draw() override;

private:
	float displacement(int i, int j, float t);
	float mW, mH, mD;
	int mResX, mResZ;
};

#pragma mark ___________________________________________________________________

// this object will make from center
class Gear: public CompoundLine {
public:
	static Gear create(const ofVec3f& p, float r, float d, float speed, int nHoles = 5, float holeRadius = 15.f, int res = 20);
	void setup(const ofVec3f& p, float r, float d,float speed,  int nHoles = 5, float holeRadius = 15.f, int res = 20);
    void draw() override;
private:
    float mSpeed {0.f};
    ofVec3f mCenter;
};


DP_SCORE_NAMESPACE_END

#endif /* dpScoreComplexLineObjects_h */
