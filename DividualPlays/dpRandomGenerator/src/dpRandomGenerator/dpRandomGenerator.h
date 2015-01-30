//
//  dpRandomGenerator.h
//  dpRandomGenerator
//
//  Created by YoshitoONISHI on 12/22/14.
//
//

#ifndef __dpRandomGenerator__dpRandomGenerator__
#define __dpRandomGenerator__dpRandomGenerator__

#include "ofMain.h"

class dpRandomGenerator {
public:
    void setup(int sourceWidth, int sourceHeight);
    void update(unsigned char* pixels);
    
    float getFloat() const; // return 0.0 - 1.0
    float getFloat(float max) const; // return 0.0 - max
    float getFloat(float min, float max) const; // return min - max
    int getInt() const; // return INT32_MIN - INT32_MAX
    float getUInt() const; // return 0 - UINT32_MAX
    ofVec3f getVec3f() const; // return spherical vector
    
private:
    class Tex {
        vector<ofVec2f> pixels;
        int w, h;
    };
    
    vector<Tex> mBuffer;
    
    int mWidth;
    int mHeight;
    size_t mBufferSize;
    size_t mWriteHead;
    size_t mReadHead;
};

#endif /* defined(__dpRandomGenerator__dpRandomGenerator__) */
