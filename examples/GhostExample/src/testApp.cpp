#include "testApp.h"


static const string myActorName = "default_unknown_date";
ramGhost ghost;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	ofBackground( ramColor::WHITE );
	
=======
	ofBackground( ramColor::WHITE-20 );

	ramGlobal().init();
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	
	/*!
	 GUI setup
	 */
	gui.setup();
	gui.loadFont("Fonts/din-webfont.ttf", 11);
	gui.loadCameraSettings("settings.camera.xml");
	
	
=======
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
	/*!
	 Ghost setup
	 */
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	ghost.setDistance(150.0);
	ghost.setSpeed(27.0);
	ghost.setHistorySize(10);
=======
	vector<ramSceneBase*> scenes;
	scenes.push_back( empty.getPtr() );
	sceneManager.setup(scenes);
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	
	
	// update ghost with passing ramActor
	ghost.update( getActor(myActorName) );
=======
	sceneManager.update();
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
}

//--------------------------------------------------------------
void testApp::draw()
{
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	// get processed ramActor ref
	ramActor& actor = ghost.getActor();

	ramCameraBegin();
	{
		ramBasicActor( actor, NULL );
	}
	ramCameraEnd();
=======
	sceneManager.draw();
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramControlPanel &gui = ramGlobal().getGUI();
	ramBasicFloor(gui.getValueI("Floor pattern"),
				  gui.getValueF("Floor size"),
				  gui.getValueF("Grid size"),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_LIGHT-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
<<<<<<< HEAD:examples/GhostExample/src/testApp.cpp
	ramBasicActor( actor, NULL );
=======
	ramControlPanel &gui = ramGlobal().getGUI();
	if ( gui.getValueB("Draw Actor") )
	{
		ramBasicActor(actor);
		
		ramGlobal().beginShadowMatrix();
		ramBasicActor(actor, ramColor::SHADOW, ramColor::SHADOW);
		ramGlobal().endShadowMatrix();
	}
>>>>>>> 38c7636e30447f1bac43815459402028cd519d87:apps/EmptyExample/src/testApp.cpp
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}



#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
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

