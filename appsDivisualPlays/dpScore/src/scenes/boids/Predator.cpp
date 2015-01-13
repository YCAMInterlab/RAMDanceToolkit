#include "Predator.h"

template<typename T>
T constrain( T val, T minVal, T maxVal )
{
	if( val < minVal ) return minVal;
	else if( val > maxVal ) return maxVal;
	else return val;
}

//Predator::Predator()
//{
//}
//
//Predator::Predator(ofVec3f pos, ofVec3f vel)
//{
//	mLen			= 5;
//	mInvLen			= 1.0f/(float)mLen;
//
//	for(int i=0; i<mLen; ++i) {
//		mPos.push_back(pos);
//	}
//	
//	mVel			= vel;
//	mVelNormal		= ofVec3f(0.f, 1.f, 0.f);
//	mAcc			= ofVec3f::zero();
//	
//	mNeighborPos	= ofVec3f::zero();
//	mNumNeighbors	= 0;
//	mMaxSpeed		= ofRandom(4.0f, 4.5f) * 2.f;
//	mMaxSpeedSqrd	= mMaxSpeed * mMaxSpeed;
//	mMinSpeed		= ofRandom(1.0f, 1.5f) * 2.f;
//	mMinSpeedSqrd	= mMinSpeed * mMinSpeed;
//
//	mColor			= ofFloatColor(1.0f, 0.0f, 0.0f, 1.0f);
//	
//	mDecay			= 0.99f;
//	mRadius			= 2.0f;
//	mLength			= 25.0f;
//	mHunger			= 1.0f;
//	
//	mIsDead			= false;
//	mIsHungry		= true;
//}
//
//void Predator::pullToCenter(const ofVec3f &center)
//{
//	ofVec3f dirToCenter	= mPos[0] - center;
//	float distToCenter	= dirToCenter.length();
//	float maxDistance	= 600.0f;
//	
//	if(distToCenter > maxDistance){
//		float pullStrength = 0.0001f;
//		mVel -= dirToCenter.normalized() * ((distToCenter - maxDistance) * pullStrength);
//	}
//}	
//
//void Predator::update(bool flatten)
//{	
//	mVel += mAcc;
//	
//	if(flatten) mAcc.z = 0.0f;
//	mVel += mAcc;
//	//mVelNormal = mVel.safeNormalized();
//    mVelNormal = mVel.normalized();
//	
//	limitSpeed();
//	
//	
//	for(int i=mLen-1; i>0; i--){
//		mPos[i] = mPos[i-1];
//	}
//	mPos[0] += mVel;
//	
//	if(flatten)
//		mPos[0].z = 0.0f;
//	
//	mVel *= mDecay;
//	
//	mAcc = ofVec3f::zero();
//	mNeighborPos = ofVec3f::zero();
//	mNumNeighbors = 0;
//	
//	mHunger += 0.001f;
//	mHunger = constrain(mHunger, 0.0f, 1.0f);
//	
//	if(mHunger > 0.5f) mIsHungry = true;
//}

void Predator::update(ofVec3f pos, ofVec3f acc)
{
    mPos = pos;
    mAcc = acc;
    
//	mHunger += 0.001f;
//	mHunger = constrain(mHunger, 0.0f, 1.0f);
//
//	if(mHunger > 0.5f) mIsHungry = true;
    mHunger = 1.0;
    mIsHungry = true;
}

//
//void Predator::limitSpeed()
//{
//	float maxSpeed = mMaxSpeed + mHunger * 3.0f;
//	float maxSpeedSqrd = maxSpeed * maxSpeed;
//	float vLengthSqrd = mVel.lengthSquared();
//	if(vLengthSqrd > maxSpeedSqrd){
//		mVel = mVelNormal * maxSpeed;
//		
//	} else if(vLengthSqrd < mMinSpeedSqrd){
//		mVel = mVelNormal * mMinSpeed;
//	}
//}
//
//void Predator::draw()
//{
//	glColor4fv(&mColor.r);
//	ofVec3f vel = mVelNormal * mLength;
//    ofLine(mPos[0] - mVel, mPos[0]);
//}
//
//void Predator::drawTail()
//{
//	glVertex3fv(mPos[0].getPtr());
//	glVertex3fv(mPos[1].getPtr());
//}
//
//void Predator::addNeighborPos(ofVec3f pos)
//{
//	mNeighborPos += pos;
//	mNumNeighbors ++;
//}
//
