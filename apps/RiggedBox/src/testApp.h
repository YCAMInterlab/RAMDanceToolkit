#pragma once

#include "ofMain.h"

#include "ramMain.h"

#include "btPicker.h"

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
    
    btPicker m_picker0;
    btPicker m_picker1;
    ofMatrix4x4 m_shadowMat;
};
