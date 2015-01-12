//
//  dpRandomGenerator.cpp
//  dpRandomGenerator
//
//  Created by YoshitoONISHI on 12/22/14.
//
//

#include "dpRandomGenerator.h"

void dpRandomGenerator::setup(int sourceWidth, int sourceHeight)
{
    mWidth = sourceWidth;
    mHeight = sourceHeight;
    
    mBuffer.clear();
    mWriteHead = 0;
    mReadHead = 0;
}

void dpRandomGenerator::update(unsigned char* pixels)
{
    
    mWriteHead++;
}

float dpRandomGenerator::getFloat() const
{
    
}

float dpRandomGenerator::getFloat(float max) const
{
    return getFloat() * max;
}

float dpRandomGenerator::getFloat(float min, float max) const
{
    return ofMap(getFloat(), 0.f, 1.f, min, max);
}

int dpRandomGenerator::getInt() const
{
    return (int)ofMap(getFloat(), 0.f, 1.f, INT32_MIN, INT32_MAX);
}

ofVec3f dpRandomGenerator::getVec3f() const
{
    
}
