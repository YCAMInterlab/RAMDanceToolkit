//
//  dpScoreShared.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreShared_h
#define dpScoreShared_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class _Shared {
public:
    _Shared(const _Shared&) = delete;
    _Shared &operator=(const _Shared&) = delete;
    ~_Shared() {}
    
    static _Shared& getInstance()
    {
        static _Shared instance;
        return instance;
    }
    
    ofFbo fbo;
    bool forFbo;
    bool focused;
    bool nextFocus;
    float t;
    float macAngleCameraUnit;
    float macAngleComputers;
    
private:
    _Shared() {}

};

#define Shared _Shared::getInstance()

DP_SCORE_NAMESPACE_END

#endif /* dpScoreShared_h */
