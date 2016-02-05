//
//  dpScoreCompound.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreCompound_h
#define dpScoreCompound_h

#include "dpScoreCommon.h"
#include "dpScoreScoped.h"
#include "dpScoreToolBox.h"

DP_SCORE_NAMESPACE_BEGIN

void compoundSetEnableCollapse(bool enable);
bool compoundGetEnableCollapse();

class PrimitiveTypeBase {
public:
    PrimitiveTypeBase();
    virtual ~PrimitiveTypeBase() = default;
    
    void update(float time);
    void transformGL();
    void reset();
    
protected:
    ofVec3f mRot;
    ofVec3f mSpd;
    ofVec3f mAng;
    ofVec3f mPos;
};

#pragma mark ___________________________________________________________________

template <class T> class Compound {
public:
    Compound() = default;
    Compound(const Compound&) = default;
    Compound& operator = (const Compound&) = default;
    virtual ~Compound() = default;
    virtual void draw();
    
protected:
    void update();
    void reset();
    
    vector<T> mTypes;
    float mStartTime;
    bool mUpdate;
};

#pragma mark ___________________________________________________________________

template <class T>
void Compound<T>::update()
{
    if (compoundGetEnableCollapse()) {
        const float t {getElapsedTime()};
        if (!mUpdate) {
            mStartTime = t;
        }
        mUpdate = true;
        const double d {t - mStartTime};
        for (auto& t : mTypes) {
            t.update(d);
        }
    }
    else {
        if (mUpdate) {
            reset();
        }
        mUpdate = false;
    }
}

template <class T>
void Compound<T>::draw()
{
    update();
    
    for (auto& t : mTypes) {
        t.draw();
    }
}

template <class T>
void Compound<T>::reset()
{
    for (auto& t : mTypes) {
        t.reset();
    }
}


DP_SCORE_NAMESPACE_END

#endif /* dpScoreCompound_h */
