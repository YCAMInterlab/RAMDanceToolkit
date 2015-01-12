#include "Particle.h"
using std::vector;

static const int TAIL_LENGTH = 500;

template<typename T>
T constrain( T val, T minVal, T maxVal )
{
	if( val < minVal ) return minVal;
	else if( val > maxVal ) return maxVal;
	else return val;
}

Particle::Particle()
{
}

Particle::Particle(ofVec3f pos, ofVec3f vel, bool followed)
{
	mPos			= pos;
	mTailPos		= pos;
	mVel			= vel;
	mVelNormal		= ofVec3f(0.f, 1.f, 0.f);
	mAcc			= ofVec3f::zero();
	
	mNeighborPos	= ofVec3f::zero();
	mNumNeighbors	= 0;
	mMaxSpeed		= ofRandom(1.0f, 2.0f);
	mMaxSpeedSqrd	= mMaxSpeed * mMaxSpeed;
	mMinSpeed		= ofRandom(0.2f, 0.5f);
	mMinSpeedSqrd	= mMinSpeed * mMinSpeed;
	
	mColor			= ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f);
		
	mDecay			= 0.99f;
	mRadius			= 3.0f;
	mLength			= 2.0f;
	mFear			= 1.0f;
	mCrowdFactor	= 1.0f;
    mSpeedMult      = 0.f;
	
	mIsDead			= false;
	mFollowed		= followed;
    
    mTailCoords.clear();
    mPointSizes.clear();
    mPointSizes.clear();
    
    for (int i=0; i<TAIL_LENGTH; i++) {
        mTailCoords.push_back(mPos);
        mPointSizes.push_back(0.f);
    }
    
    //mVaoPointSize.setData(mPointSizes.size()*sizeof(float),
    //                      &mPointSizes.at(0), sizeof(float),
    //                      GL_DYNAMIC_DRAW);
    mTail.setVertexData(&mTailCoords.at(0), TAIL_LENGTH, GL_DYNAMIC_DRAW);
}

void Particle::pullToCenter(const ofVec3f &center)
{
	ofVec3f dirToCenter = mPos - center;
	float distToCenter = dirToCenter.length();
	float distThresh = 100.0f;
	
	if(distToCenter > distThresh){
		dirToCenter.normalize();
		float pullStrength = 0.0005f;
		mVel -= dirToCenter * ((distToCenter - distThresh) * pullStrength);
	}
}

void Particle::update(bool flatten)
{	
	mCrowdFactor -= (mCrowdFactor - (1.0f - mNumNeighbors * 0.02f)) * 0.1f;
	mCrowdFactor = constrain(mCrowdFactor, 0.5f, 1.0f);
	
	mFear -= (mFear - 0.0f) * 0.2f;
	
	if(flatten)
		mAcc.z = 0.0f;
	
	mVel += mAcc;
	mVelNormal = mVel.normalized();
	
	limitSpeed();
	
	
	mPos += (mVel*mSpeedMult);
	if(flatten)
		mPos.z = 0.0f;
	
	mTailPos = mPos - (mVelNormal * mLength);
	mVel *= mDecay;
	
	float c = ::min(mNumNeighbors/50.0f, 1.0f);
	mColor.setHsb(1.0f - c, c, c * 0.5f + 0.5f, 1.0f);
	
	mAcc = ofVec3f::zero();
	mNeighborPos = ofVec3f::zero();
	mNumNeighbors = 0;
    
    while (mTailCoords.size()<=TAIL_LENGTH) {
        mTailCoords.push_back(mPos);
    }
    
    while (mTailCoords.size()>TAIL_LENGTH) {
        mTailCoords.erase(mTailCoords.begin());
    }
    
    if (mTailCoords.size() == TAIL_LENGTH) {
        mTail.updateVertexData(&mTailCoords.at(0), TAIL_LENGTH);
    }
}

void Particle::limitSpeed()
{
	float maxSpeed = mMaxSpeed + mCrowdFactor;
	float maxSpeedSqrd = maxSpeed * maxSpeed;
	
	float vLengthSqrd = mVel.lengthSquared();
	if(vLengthSqrd > maxSpeedSqrd){
		mVel = mVelNormal * maxSpeed;
		
	} else if(vLengthSqrd < mMinSpeedSqrd){
		mVel = mVelNormal * mMinSpeed;
	}
	mVel *= (1.0 + mFear);
}

void Particle::draw()
{
    ofPushStyle();
    ofSetColor(ofColor::white, 64);
    mTail.draw(GL_LINES, 0, mTailCoords.size());
    ofSetColor(ofColor::white, 128);
    mTail.draw(GL_POINTS, 0, mTailCoords.size());
    ofPopStyle();
}

void Particle::drawTail()
{
    ofSetColor(mColor);
    glVertex3fv(mPos.getPtr());
	ofSetColor(ofFloatColor(mColor.r, mColor.g, mColor.b, 0.01f));
    glVertex3fv(mTailPos.getPtr());
}

void Particle::addNeighborPos(ofVec3f pos)
{
	mNeighborPos += pos;
	mNumNeighbors ++;
}


