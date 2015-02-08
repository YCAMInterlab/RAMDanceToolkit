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
    
	float mPosEase;
	float mRad;
	float mRecordCircleScale;

    float mRecordAlpha;
	float mRecordAlphaSpeed;
    
	float mSaturation;
	float mHue;
	float mBrightness;
    
	int mPlaybackCounter;
	int mRecordCounter;
    
	bool isRecord;
	bool isRecordOnce;

    ofColor mCurrentColor;
    static const ofColor MAIN_COLOR;
    
};

#endif /* defined(__example_hakoniwa_hakoVis__dpRecordBufferCircle__) */
