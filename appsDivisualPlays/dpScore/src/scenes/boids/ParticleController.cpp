#include "ParticleController.h"
#include "dpScoreCommon.h"
#include <dispatch/dispatch.h>

using std::vector;

ParticleController::ParticleController()
{
	mPerlin = Perlin(4);
}

void ParticleController::applyForceToParticles(float zoneRadius,
                                               float lowerThresh,
                                               float higherThresh,
                                               float attractStrength,
                                               float repelStrength,
                                               float alignStrength)
{
	float twoPI = M_PI * 2.0f;
	mParticleCentroid = ofVec3f::zero();
	mNumParticles = mParticles.size();
    
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_apply(mParticles.size(), queue, ^(size_t i){

        for (int j = i+1; j<mParticles.size(); ++j){
            ofVec3f dir = mParticles.at(i).mPos - mParticles.at(j).mPos;
            float distSqrd = dir.lengthSquared();
            float zoneRadiusSqrd = zoneRadius * mParticles.at(i).mCrowdFactor * zoneRadius * mParticles.at(j).mCrowdFactor;
            
            if(distSqrd < zoneRadiusSqrd){		// Neighbor is in the zone
                float per = distSqrd/zoneRadiusSqrd;
                mParticles.at(i).addNeighborPos(mParticles.at(j).mPos);
                mParticles.at(j).addNeighborPos(mParticles.at(i).mPos);
                
                if(per < lowerThresh){			// Separation
                    float F = (lowerThresh/per - 1.0f) * repelStrength;
                    dir.normalize();
                    dir *= F;
                    
                    mParticles.at(i).mAcc += dir;
                    mParticles.at(j).mAcc -= dir;
                } else if(per < higherThresh){	// Alignment
                    float threshDelta	= higherThresh - lowerThresh;
                    float adjPer		= (per - lowerThresh)/threshDelta;
                    float F				= (1.0f - (cos(adjPer * twoPI) * -0.5f + 0.5f)) * alignStrength;
                    
                    mParticles.at(i).mAcc += mParticles.at(j).mVelNormal * F;
                    mParticles.at(j).mAcc += mParticles.at(i).mVelNormal * F;
                    
                } else {							// Cohesion (prep)
                    float threshDelta	= 1.0f - higherThresh;
                    float adjPer		= (per - higherThresh)/threshDelta;
                    float F				= (1.0f - (cos(adjPer * twoPI) * -0.5f + 0.5f)) * attractStrength;
                    
                    dir.normalize();
                    dir *= F;
                    
                    mParticles.at(i).mAcc -= dir;
                    mParticles.at(j).mAcc += dir;
                }
            }
        }
                
		mParticleCentroid += mParticles.at(i).mPos;
		/*
         if(p1->mNumNeighbors > 0){ // Cohesion
         ofVec3f neighborAveragePos = (p1->mNeighborPos/(float)p1->mNumNeighbors);
         p1->mAcc += (neighborAveragePos - p1->mPos) * attractStrength;
         }
         */
		
		// ADD PERLIN NOISE INFLUENCE
		float scale = 0.002f;
		float multi = 0.01f;
		ofVec3f perlin = mPerlin.dfBm(mParticles.at(i).mPos * scale) * multi;
		mParticles.at(i).mAcc += perlin;
		
		
		// CHECK WHETHER THERE IS ANY PARTICLE/PREDATOR INTERACTION
		float eatDistSqrd = 50.0f;
		float predatorZoneRadiusSqrd = zoneRadius * zoneRadius * 5.0f;
		for(vector<Predator>::iterator predator = mPredators.begin(); predator != mPredators.end(); ++predator) {
            
			ofVec3f dir = mParticles.at(i).mPos - predator->mPos[0];
			float distSqrd = dir.lengthSquared();
			
			if(distSqrd < predatorZoneRadiusSqrd){
				if(distSqrd > eatDistSqrd){
					float F = (predatorZoneRadiusSqrd/distSqrd - 1.0f) * 0.1f;
					mParticles.at(i).mFear += F * 0.1f;
					dir = dir.normalized() * F;
					mParticles.at(i).mAcc += dir;
					if(predator->mIsHungry)
						predator->mAcc += dir * 0.04f * predator->mHunger;
				} else {
					mParticles.at(i).mIsDead = true;
					predator->mHunger = 0.0f;
					predator->mIsHungry = false;
				}
			}
		}
		
	});
                   
	mParticleCentroid /= (float)mNumParticles;
}

void ParticleController::applyForceToPredators(float zoneRadius,
                                               float lowerThresh,
                                               float higherThresh)
{
//	float twoPI = M_PI * 2.0f;
//    
//	dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
//    
//    dispatch_apply(mPredators.size(), queue, ^(size_t i){
//
//		for(int j=i+1; j<mPredators.size(); j++) {
//			ofVec3f dir = mPredators.at(i).mPos[0] - mPredators.at(j).mPos[0];
//			float distSqrd = dir.lengthSquared();
//			float zoneRadiusSqrd = zoneRadius * zoneRadius * 4.0f;
//
//			if(distSqrd < zoneRadiusSqrd){		// Neighbor is in the zone
//				float per = distSqrd/zoneRadiusSqrd;
//				if(per < lowerThresh){			// Separation
//					float F = (lowerThresh/per - 1.0f) * 0.01f;
//					dir.normalize();
//					dir *= F;
//
//					mPredators.at(i).mAcc += dir;
//					mPredators.at(j).mAcc -= dir;
//				} else if(per < higherThresh){	// Alignment
//					float threshDelta	= higherThresh - lowerThresh;
//					float adjPer		= (per - lowerThresh)/threshDelta;
//					float F				= (1.0f - cos(adjPer * twoPI) * -0.5f + 0.5f) * 0.3f;
//
//					mPredators.at(i).mAcc += mPredators.at(j).mVelNormal * F;
//					mPredators.at(j).mAcc += mPredators.at(i).mVelNormal * F;
//
//				} else {							// Cohesion
//					float threshDelta	= 1.0f - higherThresh;
//					float adjPer		= (per - higherThresh)/threshDelta;
//					float F				= (1.0f - (cos(adjPer * twoPI) * -0.5f + 0.5f)) * 0.1f;
//
//					dir.normalize();
//					dir *= F;
//
//					mPredators.at(i).mAcc -= dir;
//					mPredators.at(j).mAcc += dir;
//				}
//			}
//		}
//	});
}

void ParticleController::pullToCenter(const ofVec3f &center)
{
	for(vector<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p){
		p->pullToCenter(center);
	}
	
//	for(vector<Predator>::iterator p = mPredators.begin(); p != mPredators.end(); ++p){
//		p->pullToCenter(center);
//	}
}

void ParticleController::update(bool flatten)
{
	for(vector<Particle>::iterator p = mParticles.begin(); p != mParticles.end();){
		if(p->mIsDead){
			p = mParticles.erase(p);
		} else {
			p->update(flatten);
			++p;
		}
	}
	
//	for(vector<Predator>::iterator p = mPredators.begin(); p != mPredators.end(); ++p){
//		p->update(flatten);
//	}
}

void ParticleController::draw()
{
    //for(vector<Predator>::iterator p = mPredators.begin(); p != mPredators.end(); ++p){
    //    float hungerColor = 1.0f - p->mHunger;
    //    //gl::color(ColorA(1.0f, hungerColor, hungerColor, 1.0f));
    //    glColor4f(1.0f, hungerColor, hungerColor, 1.0f);
    //    p->draw();
    //}
	
	for(vector<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p){
		p->draw();
	}
}

void ParticleController::addPredators(int amt)
{
	for(int i=0; i<amt; i++)
	{
		//ofVec3f pos = onc::randVec3f() * ofRandom(500.0f, 750.0f);
		//ofVec3f vel = onc::randVec3f();
		//mPredators.push_back(Predator(pos, vel));
        mPredators.push_back(Predator());
	}
}

void ParticleController::addParticles(int amt, ofVec3f offset, float min, float max)
{
	for(int i=0; i<amt; i++)
	{
		ofVec3f pos = dp::score::randVec3f() * ofRandom(min, max) + offset;
		ofVec3f vel = dp::score::randVec3f();
		
		bool followed = false;
		if(mParticles.size() == 0) followed = true;
		
		mParticles.push_back(Particle(pos, vel, followed));
	}
}

void ParticleController::removeParticles(int amt)
{
	for(int i=0; i<amt; i++)
	{
		mParticles.pop_back();
	}
}

ofVec3f ParticleController::getPos()
{
	return mParticles.begin()->mPos;
}

void ParticleController::clear()
{
    mParticles.clear();
    mPredators.clear();
}

void ParticleController::setSpeed(float f)
{
    for (auto &p : mParticles ) {
        p.mSpeedMult = f;
    }
}



