#include "VFParticle.h"

//------------------------------------------------------------
VFParticle::VFParticle(){
	setInitialCondition(0,0,0,0);
	damping = 0.03f;
	bFixed = false;
}

//------------------------------------------------------------
void VFParticle::resetForce(){
    // we reset the forces every frame
    frc.set(0,0);
}

//------------------------------------------------------------
void VFParticle::addForce(float x, float y){
    // add in a force in X and Y for this frame.
    frc.x = frc.x + x;
    frc.y = frc.y + y;
}

//------------------------------------------------------------
void VFParticle::addDampingForce(){
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
}

//------------------------------------------------------------
void VFParticle::setInitialCondition(float px, float py, float vx, float vy){
    pos.set(px,py);
	vel.set(vx,vy);
}

//------------------------------------------------------------
void VFParticle::update(){	
	if (bFixed == false){
		vel = vel + frc;
		pos = pos + vel;
        pos += ofPoint(ofRandom(-2,2),ofRandom(-2,2));
	}
}

//------------------------------------------------------------
void VFParticle::draw(){
    //ofCircle(pos.x, pos.y, 3);
    glVertex2f(pos.x, pos.y);
}


//------------------------------------------------------------
void VFParticle::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (pos.x > maxx){
		pos.x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (pos.x < minx){
		pos.x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (pos.y > maxy){
		pos.y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (pos.y < miny){
		pos.y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}
