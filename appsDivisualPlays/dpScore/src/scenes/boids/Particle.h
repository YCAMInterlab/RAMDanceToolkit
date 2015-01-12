
#pragma once
#include "ofMain.h"
#include "oncVao.h"


class Particle {
 public:
	Particle();
	Particle(ofVec3f pos, ofVec3f vel, bool followed);
	void pullToCenter(const ofVec3f &center);
	void update(bool flatten);
	void draw();
	void drawTail();
	void limitSpeed();
	void addNeighborPos(ofVec3f pos);

    float           mSpeedMult;
    
	ofVec3f         mPos;
	ofVec3f         mTailPos;
	ofVec3f         mVel;
	ofVec3f         mVelNormal;
	ofVec3f         mAcc;
	
	ofVec3f         mNeighborPos;
	int             mNumNeighbors;

	ofFloatColor	mColor;
		
	float           mDecay;
	float           mRadius;
	float           mLength;
	float           mMaxSpeed, mMaxSpeedSqrd;
	float           mMinSpeed, mMinSpeedSqrd;
	float           mFear;
	float           mCrowdFactor;

	bool            mIsDead;
	bool            mFollowed;
    
    ofVbo               mTail;
    vector<ofVec3f>     mTailCoords;
    
    vector<float>       mPointSizes;
    onc::Vao            mVaoPointSize;
};