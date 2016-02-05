//
//  dpScoreUtils.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#include "dpScoreUtils.h"

DP_SCORE_NAMESPACE_BEGIN

const string kEventAddrChangeScene {"/dp/score/changeScene"};

float clamp(float f, float range)
{
    return ofClamp(f, -range, range);
}

float getLineUped(float length,  int index, int total, bool fromCenter)
{
    const float step {length / (float)total};
    if (fromCenter)
        return -length * 0.5f + step * 0.5f + step * (float)index;
    else
        return step * 0.5f + step * (float)index;
}

ofVec3f randVec3f()
{
    const float phi {ofRandom((float)M_PI * 2.0f)};
    const float costheta {ofRandom(-1.0f, 1.0f)};
    
    const float rho {::sqrtf(1.0f - costheta * costheta)};
    const float x {rho * ::cosf(phi)};
    const float y {rho * ::sinf(phi)};
    const float z {costheta};
    
    return ofVec3f(x, y, z);
}

ofVec3f project(const ofVec3f& obj)
{
    double objX {obj.x}, objY {obj.y}, objZ {obj.z};
    double modelview[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    double projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    double winX, winY, winZ;
    
    gluProject(objX, objY, objZ, modelview, projection, viewport, &winX, &winY, &winZ);
    
    return ofVec3f(winX, viewport[3] - winY, winZ);
}

void billboard()
{
    GLdouble m[16];
    
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    m[0] = m[5] = m[10] = 1.0;
    m[1] = m[2] = m[4] = m[6] = m[8] = m[9] = 0.0;
    
    glLoadMatrixd(m);
}

static float _elapsedTime {0.f};
static bool _updateElapsedTime {true};

float getElapsedTime()
{
    return _elapsedTime;
}

void updateElapsedTime()
{
    if (_updateElapsedTime) {
        _elapsedTime += ofGetLastFrameTime();
    }
}

void setPauseElapsedTimeCounter(bool pause)
{
    _updateElapsedTime = !pause;
}

DP_SCORE_NAMESPACE_END
