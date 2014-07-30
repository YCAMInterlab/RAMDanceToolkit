//
// Quaternion.h
// RazorIMU
//
// Created by Onishi Yoshito on 2/15/13.
// Copyright (c) 2013 Onishi Yoshito. All rights reserved.
//

#ifndef RazorFreeIMU_Quaternion_h
#define RazorFreeIMU_Quaternion_h

class Quaternion {
public:
    float x, y, z, w;
    explicit Quaternion();
    explicit Quaternion(float _x, float _y, float _z, float _w);
    
    // Assuming the angles are in radians.
    void setEuler(float yaw, float pitch, float roll);
    
    //void setAngleAxis(float angle, float x, float y, float z);
    
    Quaternion operator *(const Quaternion& rhs) const;
};

#endif