#pragma once
#include "ofMain.h"
#include "Particle.h"
#include "Predator.h"
#include "Perlin.h"

class ParticleController {
 public:
	ParticleController();
	void applyForceToParticles(float zoneRadius,
                               float lowerThresh,
                               float higherThresh,
                               float attractStrength,
                               float repelStrength,
                               float orientStrength);
	void applyForceToPredators(float zoneRadius,
                               float lowerThresh,
                               float higherThresh);
	void pullToCenter(const ofVec3f &center);
	void update(bool flatten);
	void draw();
	void addParticles(int amt, ofVec3f offset, float min, float max);
	void addPredators(int amt);
	void removeParticles(int amt);
	ofVec3f getPos();
	
    void setSpeed(float f);
    
    void clear();
    
//private:
	Perlin mPerlin;
	
	std::vector<Particle>   mParticles;
	std::vector<Predator>   mPredators;
	ofVec3f                 mParticleCentroid;
	int                     mNumParticles;
};