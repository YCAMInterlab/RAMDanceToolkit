//
//  dpScoreCompoundLine.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreCompoundLine_h
#define dpScoreCompoundLine_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

// all object will make from OpenGL coord(righthanded)
struct Line {
	static Line make(const ofVec3f& p0, const ofVec3f& p1);
	void set(const ofVec3f& p0, const ofVec3f& p1);
	void draw();

	ofVec3f c;
	ofVec3f v;
};

class CompoundLine {
public:
	static bool enableAnimation;

	virtual ~CompoundLine() = default;
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

DP_SCORE_NAMESPACE_END

#endif /* dpScoreCompoundLine_h */
