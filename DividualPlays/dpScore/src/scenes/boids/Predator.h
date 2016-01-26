#pragma once
#include "ofMain.h"

class Predator {
 public:
    void update(ofVec3f pos, ofVec3f acc);
    ofVec3f mPos;
//	Predator();
//	Predator(ofVec3f pos, ofVec3f vel);
//	void pullToCenter(const ofVec3f &center);
//	void update(bool flatten);
//	void limitSpeed();
//	void draw();
//	void drawTail();
//	void addNeighborPos(ofVec3f pos);
//	
//	
//	int                     mLen;
//	float                   mInvLen;
//	std::vector<ofVec3f>    mPos;
//	
//	ofVec3f                 mVel;
//	ofVec3f                 mVelNormal;
	ofVec3f                 mAcc;
//
//	ofFloatColor            mColor;
//	
//	ofVec3f                 mNeighborPos;
//	int                     mNumNeighbors;
//	
//	float                   mDecay;
//	float                   mRadius;
//	float                   mLength;
//	float                   mMaxSpeed, mMaxSpeedSqrd;
//	float                   mMinSpeed, mMinSpeedSqrd;
	float                   mHunger;

	bool                    mIsHungry;
//	bool                    mIsDead;
};