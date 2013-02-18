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
Graph3D graph2D;
Line drawLines;
UpsideDown upsideDown;
Expansion expansion;
BigBox bigbox;
Future future;
Bullet bullet;
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
	scenes.push_back( bullet.getPtr() );
	scenes.push_back( donuts.getPtr() );
	scenes.push_back( stamp.getPtr() );
	scenes.push_back( particles.getPtr() );
	scenes.push_back( abacus.getPtr() );
	scenes.push_back( soundcube.getPtr() );
	scenes.push_back( graph3D.getPtr() );
	
	sceneManager.setup(scenes);
	
	// for 5 screens
	for (int i = 0; i < 5; i++)
	{
		ofEasyCam *cam = ramCameraManager::instance().createCamera<ofEasyCam>();
		cam->disableMouseInput();
	}
	
	for (int i = 0; i < 5; i++)
	{
		ramCameraManager::instance().setActiveCamera(i + 1);
		ramCameraManager::instance().rollbackDefaultCameraSetting(i);
	}
	
	ramCameraManager::instance().setActiveCamera(0);

}

//--------------------------------------------------------------
void testApp::update()
{
	ofViewport(0, 0, 1920, 1200);
	ramCameraManager::instance().setActiveCamera(0);
	ramBeginCamera();
	ramEndCamera();
	
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
	setDrawFloorAuto(true);
	
	int main_display_width = 1920;
	int main_display_height = 1200;
	
	main_display_width = min(main_display_width, ofGetWidth());
	main_display_height = min(main_display_height, ofGetHeight());
	
	ofPushView();
	ofViewport(0, 0, 1920, 1200);
	ramCameraManager::instance().setActiveCamera(0);
	ramBeginCamera();
	drawFloor();
	ramEndCamera();
	
	sceneManager.draw();
	ofPopView();
	
	int screen_w = 1280, screen_h = 720;
	
//	int inv_screen_height = ofGetHeight() - screen_h;
//	for (int i = 0; i < 5; i++)
//	{
//		ofPushView();
//		
//		ofCamera *screen_camera = ramCameraManager::instance().getCamera(i + 1);
//		
//		ofViewport(ofRectangle(main_display_width + i * screen_w, inv_screen_height, screen_w, screen_w));
//		ramCameraManager::instance().setActiveCamera(i + 1);
//		
//		screen_camera->begin();
//		drawFloor();
//		screen_camera->end();
//		
//		sceneManager.draw();
//		
//		ofPopView();
//	}
	
	ramCameraManager::instance().setActiveCamera(0);
	
	setDrawFloorAuto(false);
}

#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
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
