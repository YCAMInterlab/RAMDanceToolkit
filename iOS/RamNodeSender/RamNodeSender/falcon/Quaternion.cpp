//
// Quaternion.cpp
// RazorIMU
//
// Created by Onishi Yoshito on 2/15/13.
// Copyright (c) 2013 Onishi Yoshito. All rights reserved.
//

#include "Quaternion.h"
#include "math.h"

Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

void Quaternion::setEuler(float yaw, float pitch, float roll)
{
    const float c1 = cos(yaw/2);
    const float s1 = sin(yaw/2);
    const float c2 = cos(pitch/2);
    const float s2 = sin(pitch/2);
    const float c3 = cos(roll/2);
    const float s3 = sin(roll/2);
    const float c1c2 = c1*c2;
    const float s1s2 = s1*s2;
    w =c1c2*c3 - s1s2*s3;
    x =c1c2*s3 + s1s2*c3;
    y =s1*c2*c3 + c1*s2*s3;
    z =c1*s2*c3 - s1*c2*s3;
}

Quaternion Quaternion::operator *(const Quaternion& rhs) const
{
    return Quaternion(rhs.w*x + rhs.x*w + rhs.y*z - rhs.z*y,
                      rhs.w*y - rhs.x*z + rhs.y*w + rhs.z*x,
                      rhs.w*z + rhs.x*y - rhs.y*x + rhs.z*w,
                      rhs.w*w - rhs.x*x - rhs.y*y - rhs.z*z);
}