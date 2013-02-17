#include "testApp.h"


/*!
 Scenes
 */
#include "BasicActor.h"
#include "BigBox.h"
#include "Bullet.h"
#include "Future.h"
#include "Line.h"
#include "Donuts.h"
#include "Stamp.h"
#include "UpsideDown.h"
#include "Expansion.h"
#include "Graph3D.h"

BasicActor basicActor;
BigBox bigbox;
Bullet bullet;
Future future;
Line drawLines;
Donuts duplicateMe;
Stamp stamp;
UpsideDown upsideDown;
Expansion expansion;
Graph3D graph3D;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
	/// ram setup
	// ------------------
	ramInit();
	oscReceiver.setup(10000);
	
	
	/// scenes setup
	// ------------------
	vector<ramBaseScene*> scenes;
	scenes.push_back( graph3D.getPtr() );
	scenes.push_back( drawLines.getPtr() );
	scenes.push_back( basicActor.getPtr() );
	scenes.push_back( upsideDown.getPtr() );
	scenes.push_back( expansion.getPtr() );
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( future.getPtr() );
	scenes.push_back( bullet.getPtr() );
	scenes.push_back( duplicateMe.getPtr() );
	scenes.push_back( stamp.getPtr() );
	
	sceneManager.setup(scenes);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	
	/// Scenes update
	// ------------------
	sceneManager.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// Scenes draw
	// ------------------
	sceneManager.draw();
}

#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	/// Scenes drawActor
	// ------------------
	sceneManager.drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	/// Scenes drawActor
	// ------------------
	sceneManager.drawRigid(rigid);
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
