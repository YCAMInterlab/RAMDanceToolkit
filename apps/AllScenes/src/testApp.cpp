#include "testApp.h"


/*!
 Scenes
 */
#include "BigBox.h"
#include "Bullet.h"
#include "Future.h"
#include "DrawLines.h"
BigBox bigbox;
Bullet bullet;
Future future;
DrawLines drawLines;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::WHITE );
	
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
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( future.getPtr() );
	scenes.push_back( bullet.getPtr() );
	scenes.push_back( drawLines.getPtr() );
	sceneManager.setup(scenes);
}

//--------------------------------------------------------------
void testApp::update()
{
	/* Entities update */
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
	
//	ramBasicFloor(gui.getValueI("Floor pattern"),
//				  gui.getValueF("Floor size"),
//				  gui.getValueF("Grid size"),
//				  ramColor::BLUE_LIGHT,
//				  ramColor::BLUE_DEEP);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramControlPanel &gui = ramGlobal().getGUI();
//	if ( gui.getValueB("Draw Actor") )
//	{
//		ramBasicActor(actor);
//		
//		ramGlobal().beginShadowMatrix();
//		ramBasicActor(actor, ramColor::SHADOW, ramColor::SHADOW);
//		ramGlobal().endShadowMatrix();
//	}
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
		case 'b':
			bullet.cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
			break;
	}
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
