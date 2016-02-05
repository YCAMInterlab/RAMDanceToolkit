//
//  dpScoreCompoundLine.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreCompoundLine.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN


LineType::LineType(const ofVec3f& v0, const ofVec3f& v1) :
mV0(v0),
mV1(v1)
{
    
}

LineType::~LineType()
{

}

void LineType::set(const ofVec3f& v0, const ofVec3f& v1)
{
    mV0 = v0;
    mV1 = v1;
}

void LineType::draw()
{
    ScopedMatrix m;
    if (compoundGetEnableCollapse()) {
        // GPU ver
        transformGL();
        ofLine(mV0, mV1);
        // CPU ver
        //ofMatrix4x4 m;
        //m.glTranslate(mPos);
        //m.glRotate(mAng.z, 0.f, 0.f, 1.f);
        //m.glRotate(mAng.y, 0.f, 1.f, 0.f);
        //m.glRotate(mAng.x, 1.f, 0.f, 0.f);
        //const ofVec3f v0 {mV0 * m};
        //const ofVec3f v1 {mV1 * m};
        //ofLine(v0, v1);
    }
    else {
        ofLine(mV0, mV1);
    }
}

DP_SCORE_NAMESPACE_END