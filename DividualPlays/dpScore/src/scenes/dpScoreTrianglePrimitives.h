//
//  dpScoreTrianglePrimitives.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreTrianglePrimitives_h
#define dpScoreTrianglePrimitives_h

#include "dpScoreCompoundTriangle.h"

DP_SCORE_NAMESPACE_BEGIN

class TriRect: public CompoundTriangle {
public:
    static TriRect create(const ofVec3f& p, float w, float h, int resX = 1, int resY = 1);
    void setup(const ofVec3f& p, float w, float h, int resX = 1, int resY = 1);
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreTrianglePrimitives_h */
