#ifndef VFParticle_H
#define VFParticle_H

#include "ofMain.h"

class VFParticle
{
public:
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f frc;
	
	VFParticle();
	virtual ~VFParticle(){};
	
	void resetForce();
	void addForce(float x, float y);	
	void addDampingForce();
	
	void setInitialCondition(float px, float py, float vx, float vy);
	void update();
	void draw();
	
	void bounceOffWalls();
	
	bool  bFixed;
	
	float damping;
	
protected:
private:
};

#endif // VFParticle_H
