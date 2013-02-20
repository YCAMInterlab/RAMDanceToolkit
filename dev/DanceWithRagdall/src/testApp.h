#pragma once

#include "ofMain.h"

#include "ramMain.h"

class testApp : public ramBaseApp
{
public:

	// of methods
	// ------------------------
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	
	// ram methods
	// ------------------------
	void drawFloor();
    void drawActor(ramActor &actor);

    
	// ...
	// ------------------------
	ramOscReceiver oscReceiver;
    
    //enum { L, R, H, COUNT };
    enum { L, R, H, COUNT };
    
    btTypedConstraint *m_pickConstraint[COUNT];
    btVector3   m_handPos[COUNT];
    btRigidBody *m_pickedBody[COUNT];
    
};