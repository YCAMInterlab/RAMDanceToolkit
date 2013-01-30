#include "testApp.h"

/*!
 Scenes
 */
#include "EmptyScene.h"
EmptyScene empty;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::WHITE-20 );

	ramGlobal().init();
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	/*!
	 scenes setup
	 */
	vector<ramSceneBase*> scenes;
	scenes.push_back( empty.getPtr() );
	sceneManager.setup(scenes);
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	sceneManager.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	sceneManager.draw();
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
	ramControlPanel &gui = ramGlobal().getGUI();
	if ( gui.getValueB("Draw Actor") )
	{
		ramBasicActor(actor);
		
		ramGlobal().beginShadowMatrix();
		ramBasicActor(actor, ramColor::SHADOW, ramColor::SHADOW);
		ramGlobal().endShadowMatrix();
	}
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

