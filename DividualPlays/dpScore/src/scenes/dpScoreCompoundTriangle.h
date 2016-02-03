//
//  dpScoreCompoundTriangle.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreCompoundTriangle_h
#define dpScoreCompoundTriangle_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

struct Triangle {
    static Triangle make(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p3);
    void set(const ofVec3f& _p0, const ofVec3f& _p1, const ofVec3f& _p2);
    void draw();
    
    ofVec3f p0, p1, p2;
    ofVec3f c;
};

//class CompoundLine {
//public:
//    static bool enableAnimation;
//    
//    virtual ~CompoundLine() = default;
//    virtual void draw();
//    
//protected:
//    void update();
//    void reset();
//    
//    struct Point {
//        ofVec3f rot;
//        ofVec3f spd;
//        ofVec3f ang;
//        ofVec3f pos;
//    };
//    
//    vector<Line> mLines;
//    vector<Point> mPoints;
//    bool mUpdate;
//};

DP_SCORE_NAMESPACE_END


#endif /* dpScoreCompoundTriangle_h */
