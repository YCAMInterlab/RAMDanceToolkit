#pragma once

#include "ofMain.h"
#include "ramMain.h"
#include "ofxUI.h"

class testApp : public ramBaseApp
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
	void drawFloor();
	void drawActor(ramActor &actor);
	void drawRigid(ramRigidBody &rigid);
    
	
	bool showRects;
	bool showSpheres;
	bool showThreePoints, showFourPointSphere, showFourPointTwist;
	bool invertSpheres;
	bool showCircleBisector;
	bool showCenterCircles;
    float twistExtension;
	float pointSize;
	float crossLength;
	float rectRadius;
	float maxInvertRadius;
	float circleResolution;
    float twistResolution;
	
	ofxUICanvas* gui;
    ofxUIToggleMatrix *m3p, *m4p1, *m4p2, *m4p3, *m4p4;
	void setupUI();
};