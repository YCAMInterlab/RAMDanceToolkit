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

template <class T> class Compound {
public:
    virtual ~Compound() = default;
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
    
    vector<T> mTypes;
    vector<Point> mPoints;
    bool mUpdate;
};

template <class T>
void Compound<T>::update()
{
    if (compoundGetEnableCollapse()) {
        mUpdate = true;
        const double d {ofGetLastFrameTime()};
        for (auto& p : mPoints) {
            p.ang += p.rot * d;
            p.pos += p.spd * d;
            p.pos.y += 10.f * d;
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
    
    for (auto i : rep(mTypes.size())) {
        ScopedMatrix m;
        if (compoundGetEnableCollapse()) {
            auto& p = mPoints.at(i);
            ofTranslate(p.pos);
            ofRotateZ(p.ang.z);
            ofRotateY(p.ang.y);
            ofRotateX(p.ang.x);
        }
        mTypes.at(i).draw();
    }
}

template <class T>
void Compound<T>::reset()
{
    const float s {30.f};
    for (auto& p : mPoints) {
        p.rot = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
        p.spd = ofVec3f(ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s, ofRandom(-1.f, 1.f) * s);
        p.ang.set(0.f);
        p.pos.set(0.f);
    }
}


DP_SCORE_NAMESPACE_END

#endif /* dpScoreCompound_h */
