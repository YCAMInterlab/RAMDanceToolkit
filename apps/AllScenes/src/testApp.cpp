#include "testApp.h"

/*!
 GUI
 */
#include "ofxAutoControlPanel.h"
ofxAutoControlPanel gui;
ofVec3f camPos[] =
{
	ofVec3f(0, 300, 600),	/* FRONT */
	ofVec3f(600, 300, 0),	/* RIGHT */
	ofVec3f(0, 300, -600),	/* BACK */
	ofVec3f(-600, 300, 0),	/* LEFT */
	ofVec3f(0, 600, 1),		/* TOP */
	ofVec3f(0, -600, 1),	/* BOTTOM */
	ofVec3f(450, 50, 450),	/* EDGE_FR */
	ofVec3f(450, 50, -450),	/* EDGE_BR */
	ofVec3f(-450, 50, -450),/* EDGE_BL */
	ofVec3f(-450, 50, 450)	/* EDGE_FL */
};


/*!
 Scenes
 */
#include "BigBox.h"
#include "Future.h"
#include "Bullet.h"
vector<ramSceneBase*> scenes;
BigBox bigbox;
Bullet bullet;
Future future;


ofMatrix4x4 shadowMat;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColors[COLOR_WHITE]);
	
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
	
	/*!
	 gui setup
	 */
	ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 100));
	ofxControlPanel::setBackgroundColor(simpleColor(0, 0, 0, 90));
	gui.setup();
	gui.loadFont("Fonts/din-webfont.ttf", 11);
	gui.addPanel("Config");
	gui.addSlider("Background", 0, 0, 255);
	
	/* camera */
	vector<string> camPositionNames;
	camPositionNames.push_back("FRONT");
	camPositionNames.push_back("RIGHT");
	camPositionNames.push_back("BACK");
	camPositionNames.push_back("LEFT");
	camPositionNames.push_back("TOP");
	camPositionNames.push_back("BOTTOM");
	camPositionNames.push_back("EDGE_FR");
	camPositionNames.push_back("EDGE_BR");
	camPositionNames.push_back("EDGE_BL");
	camPositionNames.push_back("EDGE_FL");
	gui.addMultiToggle("Camera Position", 0, camPositionNames);
	
	/* floor */
	vector<string> floorNames;
	floorNames.push_back("PLANE");
	floorNames.push_back("CHECKER_PATTERN");
	floorNames.push_back("GRID_LINES");
	floorNames.push_back("NONE");
	gui.addMultiToggle("Floor pattern", 0, floorNames);
	gui.addSlider("Floor size", 600.0, 100.0, 1000.0);
	gui.addSlider("Grid size", 50.0, 10.0, 100.0);
	
	
	/*!
	 scenes setup
	 */
	scenes.push_back( future.getPtr() );
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( bullet.getPtr() );
		
	gui.addPanel("All Scenes");
	gui.addToggle("Draw Actor", true);
	for (int i=0; i<scenes.size(); i++)
	{
		string key = scenes.at(i)->getSceneKey();
		gui.addToggle(key, false);
	}
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->setup();
		scenes.at(i)->setMatrix(shadowMat);
		scenes.at(i)->refreshControlPanel(gui);
	}
	scenes.at(0)->enable();
	getActiveCamera().setPosition(camPos[0]);
	getActiveCamera().lookAt(ofVec3f(0,170,0));
}

//--------------------------------------------------------------
void testApp::update()
{
	
	/* Entities update */
	oscReceiver.update();
	
	
	/* Scenes update */
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->update();
	
	
	/* GUI: camera */
	if (gui.hasValueChanged("Camera Position"))
	{
		/* camera */
		int posIndex = gui.getValueI("Camera Position");
		ofVec3f pos = camPos[posIndex];
		
		getActiveCamera().setPosition(pos);
		getActiveCamera().lookAt(ofVec3f(0,170,0));
		gui.clearAllChanged();
	}
	
	
	/* GUI: floor */
	if (gui.hasValueChanged("Background"))
	{
		float bgcolor = gui.getValueF("Background");
		ofBackground(bgcolor);
		gui.clearAllChanged();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	/* Scenes draw */
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->draw();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(gui.getValueI("Floor pattern"),
				  gui.getValueF("Floor size"),
				  gui.getValueF("Grid size"),
				  ramColors[COLOR_BLUE_LIGHT],
				  ramColors[COLOR_BLUE_LIGHT]-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	if ( gui.getValueB("Draw Actor") )
	{
		ramBasicActor(actor, shadowMat.getPtr());
	}
	
	
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->drawRigid(rigid);
}





#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
//	switch (key)
//	{
//		case 'b':
//			bullet.cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
//			break;
//	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}

