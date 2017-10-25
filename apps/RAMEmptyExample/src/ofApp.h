#pragma once

// You must include `ofMain.h` and `ramMain.h`
#include "ofMain.h"
#include "ramMain.h"

// And then, include your scenes
#include "EmptyScene.h"

class ofApp : public rdtk::BaseApp
{
public:
	
	// of methods
	// ------------------------
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// ram methods
	// ------------------------
	void drawActor(const rdtk::Actor &actor);
	void drawRigid(const rdtk::RigidBody &rigid);
	void onActorSetup(const rdtk::Actor &actor);
	void onActorExit(const rdtk::Actor &actor);
	void onRigidSetup(const rdtk::RigidBody &rigid);
	void onRigidExit(const rdtk::RigidBody &rigid);

};
