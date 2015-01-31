//
//  dpRecordBufferCircle.h
//  example_hakoniwa_hakoVis
//
//  Created by kezzardrix2 on 2015/01/30.
//
//

#ifndef __example_hakoniwa_hakoVis__dpRecordBufferCircle__
#define __example_hakoniwa_hakoVis__dpRecordBufferCircle__

#include "ramMain.h"

class dpRecordBufferCircle{
public:
    
    static const int BUFFER_MAX = 500;
    
    void setup(ofPoint origin);
    void record(ofPoint pt);
    void startRecord();
    void startPlayback();
    void swapPts(ofPoint pt);
    void updateAlpha();
    void updateSaturation();
    void update();
    void draw();
    ofPoint getPos();
    
private:
    
    vector<ofVec2f>mBufs;
    
    ofPoint mOrigin;
    ofPoint mPos;
    ofPoint mTargetPos;
    
    float mPosEase = 0.1;
    float mRad = 40.0;
    float mRecordCircleScale = 2.5;

    float mRecordAlpha;
    float mRecordAlphaSpeed = 10.0;
    
    float mSaturation = 0;
    float mHue = 0;
    float mBrightness = 0;
    
    int mPlaybackCounter = 0;
    int mRecordCounter = 0;
    
    bool isRecord = false;
    bool isRecordOnce = false;
    
    ofColor mCurrentColor;
    static const ofColor MAIN_COLOR;
    
};

#endif /* defined(__example_hakoniwa_hakoVis__dpRecordBufferCircle__) */
