//
//  dpScoreLinePrimitives.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreLinePrimitives_h
#define dpScoreLinePrimitives_h

#include "dpScoreCompoundLine.h"

DP_SCORE_NAMESPACE_BEGIN

class Line: public CompoundLine {
public:
    static Line create(const ofVec3f& p0, const ofVec3f& p1, int res = 1);
    void setup(const ofVec3f& p0, const ofVec3f& p1, int res = 1);
    void update(const ofVec3f& p0, const ofVec3f& p1);
};

#pragma mark ___________________________________________________________________

class Box: public CompoundLine {
public:
	static Box create(const ofVec3f& p, float w, float h, float d);
	void setup(const ofVec3f& p, float w, float h, float d);
    float getWidth() const;
    float getHeight() const;
    float getDepth() const;
    ofVec3f getOrigin() const;
    ofVec3f getCenter() const;
private:
    ofVec3f mOrigin;
    float mW, mH, mD;
};

#pragma mark ___________________________________________________________________

class Cylinder: public CompoundLine {
public:
	static Cylinder create(const ofVec3f& p, float r, float h, int res = 16);
	void setup(const ofVec3f& p, float r, float h, int res = 16);
};

#pragma mark ___________________________________________________________________

class Rect: public CompoundLine {
public:
	static Rect create(const ofVec3f& p, float w, float h);
	void setup(const ofVec3f& p, float w, float h);
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreLinePrimitives_h */
