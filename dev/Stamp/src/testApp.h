#pragma once

#include "ofMain.h"
#include "ramMain.h"
#include "ofxXmlSettings.h"


/*!
 for debuging....
 */
static const string myActorName = "Ando_2012-09-01_18-49-10";
//static const string myActorName = "default";


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
	
    
	// ...
	// ------------------------
	vector<ramSceneBase*> scenes;
	ramOscReceiver oscReceiver;
	ramControlPanel gui;
	
	ofMatrix4x4 shadowMat;
	
	
	// Setting files
	// ------------------------
	
	ofxXmlSettings camSettingXml;
	vector<ramCameraSettings> setting_cam;
};