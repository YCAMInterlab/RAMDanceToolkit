//
//  dpScoreGadgets.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreGadgets_h
#define dpScoreGadgets_h

#include "dpScoreCommon.h"
#include "dpScoreLinePrimitives.h"

DP_SCORE_NAMESPACE_BEGIN

struct Desk {
    Desk();
    void draw();
    
    static float getWidth() { return 60.f; }
    static float getDepth() { return 180.f; }
    static float getHeight() { return 84.f; }
private:
    vector<Box> mBoxes;
};

#pragma mark ___________________________________________________________________

struct Deck {
    Deck();
    void draw();
    static float getWidth() { return 90.f; }
    static float getDepth() { return 182.f; }
    static float getHeight() { return 84.f; }
private:
    Box mTop;
    vector<Cylinder> mLegs;
};

#pragma mark ___________________________________________________________________

struct Chair {
    Chair();
    void draw();
    static float getWidth() { return 46.f; }
    static float getDepth() { return 35.f; }
private:
    vector<Box> mBoxes;
};

#pragma mark ___________________________________________________________________

struct MacBook {
    typedef void (*DrawDisplayFunc)(float w, float h);
    MacBook();
    void draw();
    static float getWidth() { return 35.5f; }
    static float getDepth() { return 24.5f; }
    float angle {110.f};
    DrawDisplayFunc drawDisplay {nullptr};
private:
    Box mBottom;
    Box mTop;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreGadgets_h */
