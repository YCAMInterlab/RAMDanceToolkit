//
//  dpScoreTypes.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#ifndef dpScoreTypes_h
#define dpScoreTypes_h

#include "dpScorePP.h"

DP_SCORE_NAMESPACE_BEGIN

using easeFunc = float(*)(float);

namespace color {
    extern const ofColor kMain;
    extern const ofColor kPalePinkLight;
    extern const ofColor kPalePinkHeavy;
    extern const ofColor kDarkPinkLight;
    extern const ofColor kDarkPinkHeavy;
}

typedef unsigned char byte;

union floatByte {
    float f;
    byte b[4];
};

union floatInt {
    float f;
    int i;
};

union intByte {
    int i;
    byte b[4];
};

struct OnOffNumpunct : numpunct<char> {
    using numpunct<char>::string_type;
    string_type do_truename()  const { return "on "; }
    string_type do_falsename() const { return "off"; }
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreTypes_h */
