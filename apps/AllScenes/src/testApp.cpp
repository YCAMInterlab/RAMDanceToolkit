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
#include "Graph2D.h"
#include "Particles.h"
#include "Abacus.h"
#include "SoundCube.h"

BasicActor basicActor;
Graph3D graph3D;
Graph2D graph2D;
Line drawLines;
UpsideDown upsideDown;
Expansion expansion;
BigBox bigbox;
Future future;
Donuts donuts;
Stamp stamp;
Particles particles;
Abacus abacus;
SoundCube soundcube;

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
	scenes.push_back( basicActor.getPtr() );
	scenes.push_back( drawLines.getPtr() );
	scenes.push_back( upsideDown.getPtr() );
	scenes.push_back( expansion.getPtr() );
	scenes.push_back( bigbox.getPtr() );
	scenes.push_back( future.getPtr() );
	scenes.push_back( donuts.getPtr() );
	scenes.push_back( stamp.getPtr() );
	scenes.push_back( particles.getPtr() );
	scenes.push_back( abacus.getPtr() );
	scenes.push_back( soundcube.getPtr() );
	scenes.push_back( graph3D.getPtr() );
	scenes.push_back( graph2D.getPtr() );
	
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
